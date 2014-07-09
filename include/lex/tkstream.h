#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <functional>
#include <iostream>

#include "lex/tokenfsm.h"

class lex_exception;
class token;

class tokenizer
{
public:
    tokenizer(std::istream &ins);
    ~tokenizer();
    
    bool good() const;
    operator bool();
    
    void on_lex_exception(const std::function<void (lex_exception&)> &);
    
    friend tokenizer& operator>>(tokenizer &tkz, token &tk);
    
protected:
    token next();
    
private:
    std::istream &_ins;
    
    Token_fsm _fsm;
    
    /**
     * true if the last attempt retrieve data succeed.
     */
    bool _good;
    
    bool _eof;
    
    void displayStatus(bool);
};
tokenizer& operator>>(tokenizer &tkz, token &tk);

#endif // TOKENIZER_H
