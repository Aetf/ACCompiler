#include <cstdio>
#include <iostream>
#include <string>
#include <boost/concept_check.hpp>

#include "lex/lexexception.h"
#include "lex/token.h"
#include "lex/tkstream.h"

using std::istream;
using std::string;

tkstream::tkstream(std::istream &ins)
    :_ins(ins)
{
    _good = true;
    _eof = false;
    _fillbuf = true;
    _fsm.initiate();
    _fsm.on_lex_exception += [&](lex_exception &ex) {
    };
}

tkstream::~tkstream()
{
}

bool tkstream::good() const
{
    return _good;
}

tkstream::operator bool() const
{
    return good();
}

token tkstream::peek()
{
    if(_fillbuf) {
        try
        {
            fillbuf();
        }
        catch (std::ios_base::failure) {}
        catch (lex_exception) {}
    }
    
    return _tkbuf;
}

token tkstream::advance()
{
    token tk;
    *this >> tk;
    return tk;
}

void tkstream::on_lex_exception(const std::function< void (lex_exception&)>& handler)
{
    _fsm.on_lex_exception += handler;
}

void tkstream::fillbuf()
{
    if(_eof)
    {
        _good = false;
        throw std::ios_base::failure("End of file.");
    }
    
    const StToken* state = NULL;
    try
    {
        _fsm.process_event(EvReset());
        while( NULL == (state = _fsm.state_cast<const StToken*>()))
        {
            _fsm.process_event(EvChar(_ins.get()));
        }
    }
    catch(end_of_input_exception &ex)
    {
        // got eof, this can only be thrown in StBlank,
        // so no more data is in fsm.
        _eof = true;
        // set good to false safely.
        _good = false;
        // since here is no data to return,
        // throw an exception to say we are at EOF.
        throw std::ios_base::failure("End of file");
    }
    catch(lex_exception &ex)
    {
        _good = false;
        // rethrow to return
        throw;
    }
    
    _tkbuf = state->gettoken();
    _fillbuf = false;
}

token tkstream::next()
{
    if(_fillbuf) {
        fillbuf();
    }
    
    _fillbuf = true;
    return _tkbuf;
}

tkstream& operator>>(tkstream& tkz, token& tk)
{
    try
    {
        tk = tkz.next();
    }
    catch (lex_exception&)
    {
        // already handled in _handler.
    }
    catch (std::ios_base::failure&)
    {
        // end of file.
    }
    return tkz;
}

void tkstream::displayStatus(bool before)
{
    std::cout << "Display status " << (before?"before":"after") << " enter:"<< std::endl;
    std::cout << "\tword: " << _fsm.word() << std::endl;
    std::cout << "\tpos: " << _fsm.current_pos().line <<":"<<_fsm.current_pos().col << std::endl;
    for(auto it = _fsm.state_begin(); it != _fsm.state_end(); it++)
    {
        std::cout << "\tstate: " << typeid(*it).name() << std::endl;
    }
}

