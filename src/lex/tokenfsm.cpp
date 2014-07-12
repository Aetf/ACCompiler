#include <cstdio>

#include <boost/concept_check.hpp>

#include "lex/lexexception.h"
#include "lex/textpointer.h"
#include "lex/token.h"
#include "lex/tokenfsm.h"

string Token_fsm::word() const
{
    return word_;
}

void Token_fsm::append(char ch)
{
    word_ += ch;
}

void Token_fsm::reset()
{
    word_.clear();
}

const text_pointer& Token_fsm::current_pos() const
{
    return pos_;
}

bool Token_fsm::update_pos(int ch)
{
    static bool needclear = false;
    if(needclear)
    {
        needclear = false;
        
        // throw any deffered exceptions
        for (auto exp : deffered_ex_)
        {
            // we have a complete line now,
            // replace the old one
            exp->line_ = line_;
            try
            {
                on_lex_exception(*exp);
            } catch (...) {}
        }
        deffered_ex_.clear();
        
        line_ = "";
        
        capturingCommentBlkStart_ = false;
        capturingQuoteStart_ = false;
    }
    
    bool islast = pos_.step(ch);
    if(islast)
    {
        needclear = true;
    }
    
    line_ += ch;
    if(capturingCommentBlkStart_)
        commentBlkStartLine_ += ch;
    if(capturingQuoteStart_)
        quoteStartLine_ += ch;
    
    return islast;
}

void Token_fsm::start_comment_blk()
{
    if(capturingCommentBlkStart_)
        return;
    
    capturingCommentBlkStart_ = true;
    commentBlkStartLine_ = line_;
    commentBlkStartPos_ = pos_;
}

void Token_fsm::end_comment_blk()
{
    capturingCommentBlkStart_ = false;
    commentBlkStartLine_ = "";
}

const text_pointer& Token_fsm::last_comment_blk() const
{
    return commentBlkStartPos_;
}

void Token_fsm::start_quote()
{
    if(capturingQuoteStart_)
        return;
    capturingQuoteStart_ = true;
    quoteStartLine_ = line_;
    quoteStartPos_ = pos_;
}

void Token_fsm::end_quote()
{
    capturingQuoteStart_ = false;
    quoteStartLine_ = "";
}

const text_pointer& Token_fsm::last_quote() const
{
    return quoteStartPos_;
}

void Token_fsm::raise_illegal_character(int ch)
{
    try
    {
        auto ex = new illegal_char_exception(ch, current_pos(), line_);
        // not sure we have a full line, defered the exception
        deffered_ex_.push_back(ex);
    } catch(...) {}
}

void Token_fsm::raise_missing_quote()
{
    try
    {
        if (!deffered_ex_.empty()) {
            // there are some exception deffered,
            // must deffer this one too to preserve order
            auto ex = new missing_quote_exception(quoteStartPos_, quoteStartLine_);
            deffered_ex_.push_back(ex);
        } else {
            missing_quote_exception ex(quoteStartPos_, quoteStartLine_);
            on_lex_exception(ex);
        }
    } catch(...) {}
}

void Token_fsm::raise_missing_squote()
{
    try
    {
        auto ex = new missing_squote_exception(current_pos(), line_);
        // not sure we have a full line, defered the exception
        deffered_ex_.push_back(ex);
    } catch(...) {}
}

void Token_fsm::raise_unterminated_comment()
{
    try
    {
        if (!deffered_ex_.empty()) {
            // there are some exception deffered,
            // must deffer this one too to preserve order
            auto ex = new unterminated_comment_exception(commentBlkStartPos_,
                                                  commentBlkStartLine_);
            deffered_ex_.push_back(ex);
        } else {
            unterminated_comment_exception ex(commentBlkStartPos_,
                                       commentBlkStartLine_);
            on_lex_exception(ex);
        }
    } catch(...) {}
}

sc::result StBlank::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isspace(ch))
    {
        // ignore the char, but we also have to update position.
        outermost_context().update_pos(ch);
        return discard_event();
    } else if(token::isalpha(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StAlphaNum>();
        
    } else if(token::isdigit(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StNumber>();
        
    } else if(token::isquote(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        outermost_context().start_quote();
        return transit<StString>();
        
    } else if(token::issquote(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StChar>();
        
    } else if(token::isslash(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StSlash>();
        
    } else if(token::OP_RPAREN(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StOp_Partial>();
    } else if(token::isdlchar(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StDl_Partial>();
    } else if(token::iseof(ch))
    {
       throw end_of_input_exception(); 
    }
    else
    {
        // don't append ch, which may be non char.
        // so we have to update text_pointer manully.
        outermost_context().update_pos(ch);
        
        outermost_context().raise_illegal_character(ch);
        return discard_event();
    }
}

sc::result StAlphaNum::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isalnum(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    }
    else
    {
        // we shouldn't consume this event,
        // re-post it to the queue.
        post_event(evt);
        if(token::isreserved(context<Token_fsm>().word()))
        {
            return transit<StToken_Reserved>();
        } else
        {
            return transit<StToken_Identifier>();
        }
    }
}

sc::result StNumber::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isdigit(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    } else if(token::isdot(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StNumber_Has_Dot>();
    } else
    {
        // we shouldn't consume this event,
        // re-post it to the queue.
        post_event(evt);
        return transit<StToken_Int>();
    }
}

sc::result StNumber_Has_Dot::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isdigit(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    } else
    {
        // we shouldn't consume this event, re-post it to the queue.
        post_event(evt);
        return transit<StToken_Float>();
    }
}

sc::result StString::react(const EvChar &evt)
{
    char ch = evt.what();
    
    // update position, and use the return value
    // to determine if we are at the end of a line.
    bool islast = outermost_context().update_pos(ch);
    if(token::isescapse(ch))
    {
        // this is an backslash, indicates the next char should be escaped,
        // no need to append it.
        return transit<StString_ESCP>();
    } else if(token::isquote(ch))
    {
        outermost_context().append(ch);
        outermost_context().end_quote();
        return transit<StToken_String>();
    } else if(islast)
    {
        //outermost_context().append(ch);
        
        outermost_context().raise_missing_quote();
        // try to recovery from the error
        // if the string as some contents, add a '"'
        // otherwise skip it
        if(outermost_context().word().length() > 1)
        {
            outermost_context().append('"');
            return transit<StToken_String>();
        }
        else
        {
            // discard the word cache and restart.
            outermost_context().reset();
            return transit<StBlank>();
        }
    }
    else
    {
        outermost_context().append(ch);
        return discard_event();
    }
}

sc::result StChar::react(const EvChar &evt)
{
    char ch = evt.what();
    
    // update position, and use the return value
    // to determine if we are at the end of a line.
    bool islast = outermost_context().update_pos(ch);
    if(token::isescapse(ch))
    {
        // this is an backslash, indicates the next char should be escaped,
        // no need to append it.
        return transit<StString_ESCP>();
    } else if(token::issquote(ch))
    {
        outermost_context().append(ch);
        return transit<StToken_Char>();
    } else if(islast)
    {
        //outermost_context().append(ch);
        
        outermost_context().raise_missing_squote();
        // try to recovery from the error
        // if the string as some contents, add a '"'
        // otherwise skip it
        if(outermost_context().word().length() > 1)
        {
            outermost_context().append('\'');
            return transit<StToken_Char>();
        }
        else
        {
            // discard the word cache and restart.
            outermost_context().reset();
            return transit<StBlank>();
        }
    }
    else
    {
        // char only allow one character between squote
        if(outermost_context().word().length() >= 2)
        {
            // error
            outermost_context().raise_missing_squote();
            // discard the word cache and restart.
            outermost_context().reset();
            return transit<StBlank>();
        }
        outermost_context().append(ch);
        return discard_event();
    }
}

sc::result StString_ESCP::react(const EvChar &evt)
{
    char ch = evt.what();
    // should use the literal value not the escaped value to update position.
    bool islast = outermost_context().update_pos(evt.what());
    
    // we are at the end of the line, linefeeds should not appear in str literal.
    if(token::islinefeedchar(ch))
    {
        if(islast)
        {
            return transit<StString>();
        }
        else // but not finished the linefeed yet.
        {
            return transit<StString_ESCP>();
        }
    }
    
    outermost_context().append(token::escape(ch));
    return transit<StString>();
}

sc::result StSlash::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isstar(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        outermost_context().start_comment_blk();
        return transit<StComment_Block>();
    } else if(token::isslash(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StComment_Line>();
    } else
    {
        // we shouldn't consume this event, re-post it to the queue.
        post_event(evt);
        return transit<StToken_Operator>();
    }
}

sc::result StComment_Block::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isstar(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StComment_Pre_End>();
    } else if (token::iseof(ch))
    {
        // must handle EOF
        // a special case in which the comment
        // block failed to close.
        outermost_context().raise_unterminated_comment();
        throw end_of_input_exception();
    }
    else
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    }
}

sc::result StComment_Pre_End::react(const EvChar &evt)
{
    char ch = evt.what();
    if(token::isslash(ch))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        outermost_context().end_comment_blk();
        return transit<StToken_Comment>();
    } else
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return transit<StComment_Block>();
    }
}

sc::result StComment_Line::react(const EvChar &evt)
{
    char ch = evt.what();
    bool islast = outermost_context().update_pos(ch);
    if(islast)
    {
        return transit<StToken_Comment>();
    } else
    {
        outermost_context().append(ch);
        return discard_event();
    }
}

sc::result StOp_Partial::react(const EvChar &evt)
{
    char ch = evt.what();
    string text = outermost_context().word() + ch;
    // as long as text is operator, we accept it.
    // i.e. the longest matched operator.
    // so we can recoganize := as a whole rather than : and =
    if(token::isoperator(text))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    } else
    {
        // we shouldn't consume this event, re-post it to the queue.
        post_event(evt);
        return transit<StToken_Operator>();
    }
}

sc::result StDl_Partial::react(const EvChar &evt)
{
    char ch = evt.what();
    string text = outermost_context().word() + ch;
    // the same as operator
    if(token::isdelimiter(text))
    {
        outermost_context().update_pos(ch);
        outermost_context().append(ch);
        return discard_event();
    } else
    {
        // we shouldn't consume this event, re-post it to the queue.
        post_event(evt);
        return transit<StToken_Delimiter>();
    }
}

token StToken_Identifier::gettoken() const
{
    token tk;
    tk.id(token_id::IDENTIFIER);
    tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_Reserved::gettoken() const
{
    token tk;
    const string &w = outermost_context().word();
    tk.id(token::id_reserved(w));
    // tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_Int::gettoken() const
{
    token tk;
    tk.id(token_id::CONST_INT);
    tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_Float::gettoken() const
{
    token tk;
    tk.id(token_id::CONST_FLOAT);
    tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_String::gettoken() const
{
    token tk;
    tk.id(token_id::CONST_STR);
    tk.text(outermost_context().word());
    tk.position(outermost_context().last_quote());
    
    return tk;
}

token StToken_Char::gettoken() const
{
    token tk;
    tk.id(token_id::CONST_CHAR);
    tk.text(outermost_context().word());
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - 3
    ));
    
    return tk;
}

token StToken_Operator::gettoken() const
{
    token tk;
    const string &w = outermost_context().word();
    tk.id(token::id_operator(w));
    tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_Delimiter::gettoken() const
{
    token tk;
    const string &w = outermost_context().word();
    tk.id(token::id_delimiter(w));
    tk.text(outermost_context().word());
    
    int length = outermost_context().word().length();
    tk.position(text_pointer(
        outermost_context().current_pos().line,
        outermost_context().current_pos().col - length + 1
    ));
    
    return tk;
}

token StToken_Comment::gettoken() const
{
    token tk;
    tk.id(token_id::COMMENT);
    tk.text(outermost_context().word());
    tk.position(outermost_context().last_comment_blk());
    
    return tk;
}