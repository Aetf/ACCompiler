#ifndef LEXEXCEPTION_H
#define LEXEXCEPTION_H

#include <exception>
#include <string>

#include "lex/textpointer.h"

using std::exception;
using std::string;

struct Token_fsm;
class lex_exception : exception
{
public:
    friend Token_fsm;
    
    lex_exception(const text_pointer &pos = text_pointer(), const string &line = "");
    virtual ~lex_exception() throw();
    
    virtual const text_pointer& where() const throw();
    virtual const char* relavent_line() const throw();
    
    virtual const char* to_string() const throw();
    
protected:
    text_pointer pos_;
    string line_;
};

class missing_quote_exception : public lex_exception
{
public:
    missing_quote_exception(const text_pointer &pos = text_pointer(),
                            const string &line = "");
    
    virtual const char* what() const throw() override;
};

class unterminated_comment_exception : public lex_exception
{
public:
    unterminated_comment_exception(const text_pointer &pos = text_pointer(),
                                   const string &line = "");
    
    virtual const char* what() const throw() override;
};

class illegal_char_exception : public lex_exception
{
public:
    illegal_char_exception(int curr, const text_pointer &pos = text_pointer(),
                           const string &line = "")
    : lex_exception(pos, line), current_(curr) {}
    
    int current() const throw();
    
    virtual const char* what() const throw() override;
    
private:
    int current_;
};

class end_of_input_exception : public lex_exception
{
public:
    end_of_input_exception()
    : lex_exception() {}
};

#endif // LEXEXCEPTION_H