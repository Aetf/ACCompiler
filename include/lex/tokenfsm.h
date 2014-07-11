#ifndef TOKEN_FSM_H
#define TOKEN_FSM_H

#include <string>
#include <vector>

#include <boost/mpl/list.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deferral.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>

#include "delegate.h"
#include "lex/textpointer.h"

namespace mpl = boost::mpl;
namespace sc = boost::statechart;

class lex_exception;
class token;

/**
 * Event that represents a char input
 */
struct EvChar : sc::event<EvChar>
{
    EvChar(char ch) : ch_(ch) {}
    int what() const { return ch_; }
private:
    char ch_;
};

/**
 * Event used to reset fsm breaking StToken state.
 */
struct EvReset : sc::event<EvReset> { };

/**
 * Out FSM defined here.
 * Initial state: StValid
 */
using LexExceptionHandler = delegate<void, lex_exception&>;
struct StValid;
struct Token_fsm : sc::state_machine<Token_fsm, StValid>
{
public:
    /**
     * @Return a string contains all currently accepted chars.
     * This is regularly cleard after leaving final states
     */
    std::string word() const;
    
    /**
     * Append a char to the current accept string.
     * Note: Must call update_pos to update the inner text_pointer.
     */
    void append(char ch);
    
    /**
     * Reset the fsm to initial state.
     */
    void reset();
    
    /**
     * @Return a text_pointer points at the position
     * of current accepted char in a file.
     */
    const text_pointer& current_pos() const;
    
    /**
     * update the inner text_pointer according to the given char.
     * @Return true if the ch is the last char of a line.
     */
    bool update_pos(int ch);
    
    /**
     * starts a quote
     */
    void start_quote();
    
    /**
     * ends a quote
     */
    void end_quote();
    
    /**
     * @Return a text pointer points to the
     * last string quote start position.
     */
    const text_pointer& last_quote() const;
    
    /**
     * starts a comment block
     */
    void start_comment_blk();
    
    /**
     * ends a comment block
     */
    void end_comment_blk();
    
    /**
     * @Return a text pointer points to the
     * last comment block start position.
     */
    const text_pointer& last_comment_blk() const;
    
    /**
     * Exception event handler.
     */
    LexExceptionHandler on_lex_exception;
    
    /**
     * Raise an exception.
     * Will automatic add line and position info.
     */
    void raise_illegal_character(int ch);
    void raise_missing_squote();
    void raise_missing_quote();
    void raise_unterminated_comment();
    
private:
    std::string word_;
    text_pointer pos_;
    std::string line_;
    
    text_pointer quoteStartPos_;
    std::string quoteStartLine_;
    bool capturingQuoteStart_ = false;
    
    text_pointer commentBlkStartPos_;
    std::string commentBlkStartLine_;
    bool capturingCommentBlkStart_ = false;
    
    std::vector<lex_exception*> deffered_ex_;
};

/**
 * Defination of StValid.
 * Inner initial state: StBlank
 */
struct StBlank;
struct StValid : sc::simple_state<StValid, Token_fsm, StBlank>
{};

/**
 * Defination of StInValid.
 * Represents an invalid state of fsm.
 * NOT USED CURRENT
 */
struct StInValid : sc::simple_state<StInValid, Token_fsm>
{
};

/**
 * Definations of all non-final states
 * goes here.
 * Use #define to ease the task.
 */
#define DECLEAR_STATE_CLASS(class) \
struct class : sc::simple_state<class, StValid> \
{ \
public: \
    typedef mpl::list< \
        sc::custom_reaction<EvChar> \
        > reactions; \
    sc::result react(const EvChar &evt); \
}; \
/**/
DECLEAR_STATE_CLASS(StBlank)
DECLEAR_STATE_CLASS(StAlphaNum)
DECLEAR_STATE_CLASS(StNumber)
DECLEAR_STATE_CLASS(StNumber_Has_Dot)
DECLEAR_STATE_CLASS(StString)
DECLEAR_STATE_CLASS(StChar)
DECLEAR_STATE_CLASS(StString_ESCP)
DECLEAR_STATE_CLASS(StSlash)
DECLEAR_STATE_CLASS(StComment_Block)
DECLEAR_STATE_CLASS(StComment_Pre_End)
DECLEAR_STATE_CLASS(StComment_Line)
DECLEAR_STATE_CLASS(StOp_Partial)
DECLEAR_STATE_CLASS(StDl_Partial)
#undef DECLEAR_STATE_CLASS

/**
 * Definations of final states goes
 * here.
 * They are all derived from a commen base
 * StToken, so that we can retrieve the token
 * from the state.
 */
struct StToken
{
    virtual token gettoken() const = 0;
};
#define DECLEAR_FINAL_STATE_CLASS(class) \
struct class : sc::simple_state<class, StValid>, StToken \
{ \
public: \
    typedef mpl::list< \
        sc::deferral<EvChar>, \
        sc::custom_reaction<EvReset> \
        > reactions; \
    sc::result react(const EvReset &evt) \
    { \
        outermost_context().reset(); \
        return transit<StBlank>(); \
    } \
    token gettoken() const override; \
}; \
/**/
DECLEAR_FINAL_STATE_CLASS(StToken_Identifier)
DECLEAR_FINAL_STATE_CLASS(StToken_Reserved)
DECLEAR_FINAL_STATE_CLASS(StToken_Int)
DECLEAR_FINAL_STATE_CLASS(StToken_Float)
DECLEAR_FINAL_STATE_CLASS(StToken_Char)
DECLEAR_FINAL_STATE_CLASS(StToken_String)
DECLEAR_FINAL_STATE_CLASS(StToken_Operator)
DECLEAR_FINAL_STATE_CLASS(StToken_Delimiter)
DECLEAR_FINAL_STATE_CLASS(StToken_Comment)
#undef DECLEAR_FINAL_STATE_CLASS

#endif // TOKEN_FSM_H