#include <sstream>

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "lex/lexexception.h"
#include "lex/token.h"

using std::endl;
using std::ostringstream;

lex_exception::lex_exception(const text_pointer &pos,
                             const string &line)
    :pos_(pos), line_(line)
{
    boost::erase_all(line_, token::curr_linefeed());
}

lex_exception::~lex_exception() throw()
{}

const char* lex_exception::relavent_line() const throw()
{
    return line_.c_str();
}

const char* lex_exception::to_string() const throw()
{
    
    ostringstream os;
    os << where().line << ":" << where().col << ": error: " << what() << '\n'
       << boost::replace_all_copy(line_, "\t", " ") << '\n'
       << string(where().col-1, ' ') << '^';
    return os.str().c_str();
}

const text_pointer& lex_exception::where() const throw()
{
    return pos_;
}


missing_quote_exception::missing_quote_exception(const text_pointer &pos,
                        const string &line)
:lex_exception(pos, line)
{
    
}

const char* missing_quote_exception::what() const throw()
{
    return "missing terminating '\"' character";
}

missing_squote_exception::missing_squote_exception(const text_pointer &pos,
                                                 const string &line)
:lex_exception(pos, line)
{
    
}

const char* missing_squote_exception::what() const throw()
{
    return "missing terminating '\'' character";
}

unterminated_comment_exception::unterminated_comment_exception(const text_pointer &pos,
                                                               const string &line)
:lex_exception(text_pointer(pos.line, pos.col - 1), line)
{
    pos_ = text_pointer(pos.line, pos.col - 1);
}

const char* unterminated_comment_exception::what() const throw()
{
    return "unterminated /* comment";
}

const char* illegal_char_exception::what() const throw()
{
    return ("Character " + string(1, current_) + " is not allowed.").c_str();
}

int illegal_char_exception::current() const throw()
{
    return current_;
}