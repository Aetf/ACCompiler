#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <functional>
#include <iostream>

#include "lex/tokenfsm.h"

class lex_exception;
class token;

class tkstream
{
public:
    tkstream(std::istream &ins);
    ~tkstream();
    
    bool good() const;
    operator bool() const;
    
    token peek();
    
    void on_lex_exception(const std::function<void (lex_exception&)> &);
    
    friend tkstream& operator>>(tkstream &tkz, token &tk);
    
protected:
    token next();
    void fillbuf();
    
private:
    std::istream &_ins;
    
    Token_fsm _fsm;
    
    /**
     * true if the last attempt retrieve data succeed.
     */
    bool _good;
    
    bool _eof;
    
    bool _fillbuf;
    token _tkbuf;
    
    void displayStatus(bool);
};
tkstream& operator>>(tkstream &tkz, token &tk);

#endif // TOKENIZER_H
