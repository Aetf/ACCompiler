#include <algorithm>
#include <cctype>

#include <boost/algorithm/string.hpp>

#include "lex/token.h"

/**
 * constant tokens dictionary
 */
linefeed_type token::_lftype;
reserved_dict token::_reserved;
operators_dict token::_operators;
delimiter_dict token::_delimiter;
escape_dict token::_escape;
linefeed_dict token::_linefeed;

token::token()
{
    _id = token_id::ILLIGEL;
}

token_id token::id() const
{
    return _id;
}

void token::id(const token_id& tid)
{
    _id = tid;
}

text_pointer token::position() const
{
    return _pos;
}

void token::position(const text_pointer& pointer)
{
    _pos = pointer;
}

const string& token::text() const
{
    return _text;
}

void token::text(const string& str)
{
    _text = str;
}

bool token::good() const
{
    return _id != token_id::ILLIGEL;
}

bool token::iscontant() const
{
    int id = static_cast<int>(_id);
    return id >= 33 && id <= 36;
}

bool token::isdelimiter() const
{
    int id = static_cast<int>(_id);
    return id >= 28 && id <= 32;
}

bool token::isidentifier() const
{
    int id = static_cast<int>(_id);
    return id >= 38 && id <= 38;
}

bool token::isoperator() const
{
    int id = static_cast<int>(_id);
    return id >= 14 && id <= 27;
}

bool token::isreserved() const
{
    int id = static_cast<int>(_id);
    return id >= 1 && id <= 13;
}

bool token::iscomment() const
{
    int id = static_cast<int>(_id);
    return id >= 37 && id <= 37;
}

const dictionary& token::reserved()
{
    return _reserved;
}

const dictionary& token::operators()
{
    return _operators;
}

const dictionary& token::delimiter()
{
    return _delimiter;
}


bool token::isdelimiter(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return token::delimiter().find(str) != token::delimiter().end();
}

token_id token::id_delimiter(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return delimiter().at(str);
}

bool token::isoperator(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return token::operators().find(str) != token::operators().end();
}

token_id token::id_operator(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return operators().at(str);
}

bool token::isreserved(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return token::reserved().find(str) != token::reserved().end();
}

token_id token::id_reserved(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return reserved().at(str);
}

bool token::islinefeed(string str)
{
    return str == curr_linefeed();
}

void token::set_linefeed(linefeed_type type)
{
    _lftype = type;
}

string token::curr_linefeed()
{
    return _linefeed[_lftype];
}

bool token::isalnum(int ch)
{
    return std::isalnum(ch);
}

bool token::isalpha(int ch)
{
    return std::isalpha(ch);
}

bool token::isdigit(int ch)
{
    return std::isdigit(ch);
}

bool token::islinefeedchar(int ch)
{
    return ch == '\n' || ch == '\r';
}

bool token::isquote(int ch)
{
    return ch == '"';
}

bool token::isdot(int ch)
{
    return ch == '.';
}

bool token::isescapse(int ch)
{
    return ch == '\\';
}

bool token::isopfront(int ch)
{
    for(auto entry : token::operators())
    {
        if(entry.first.find_first_of(ch) != string::npos)
            return true;
    }
    return false;
}

bool token::isdlfront(int ch)
{
    for(auto entry : token::delimiter())
    {
        if(entry.first.find_first_of(ch) != string::npos)
            return true;
    }
    return false;
}

bool token::isslash(int ch)
{
    return ch == '/';
}

bool token::isspace(int ch)
{
    return std::isspace(ch);
}

bool token::isstar(int ch)
{
    return ch == '*';
}

bool token::iseof(int ch)
{
    return ch == EOF;
}

int token::escape(int ch)
{
    auto it = _escape.find(string(1, (char)ch));
    return (it == _escape.end()) ? ch : it->second;
}