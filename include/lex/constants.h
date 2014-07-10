#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <ostream>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

enum class token_id : int
{
    ILLIGEL,
    REV_PROGRAM, REV_BEGIN, REV_END, REV_VAR,
    REV_INT, REV_FLOAT, REV_CHAR,
    REV_AND, REV_OR, REV_NOT, REV_IF, REV_THEN, REV_ELSE,
    REV_WHILE, REV_DO, REV_FOR,
    OP_ADD, OP_MUL, OP_SUB, OP_DIV,
    OP_ADDR,
    OP_LBRAC, OP_RBRAC, OP_DOT, OP_LSBRAC, OP_RSBRAC,
    OP_ASSIGN,
    OP_GT, OP_LT, OP_EQ, OP_GE, OP_LE, OP_NE,
    DELIM_COMMA, DELIM_SEMI,
    DELIM_COMMENT_INL, DELIM_COMMENT_BEG, DELIM_COMMENT_END,
    CONST_INT, CONST_FLOAT, CONST_STR, CONST_BOOL,
    COMMENT,
    IDENTIFIER
};

std::ostream& operator<<(std::ostream &os, const token_id &id);

enum class linefeed_type : short
{
    Unix, Windows, OldMac
};

struct hash_linefeed_type
{
    size_t operator() (linefeed_type t) const { return std::hash<short>()(static_cast<short>(t)); }
};

using dictionary = unordered_map<string, token_id>;

class reserved_dict : public dictionary
{
public:
    reserved_dict()
    {
        (*this)["program"] = token_id::REV_PROGRAM;
        (*this)["{"] = token_id::REV_BEGIN;
        (*this)["}"] = token_id::REV_END;
        (*this)["var"] = token_id::REV_VAR;
        (*this)["int"] = token_id::REV_INT;
        (*this)["float"] = token_id::REV_FLOAT;
        (*this)["char"] = token_id::REV_CHAR;
        (*this)["&&"] = token_id::REV_AND;
        (*this)["||"] = token_id::REV_OR;
        (*this)["!"] = token_id::REV_NOT;
        (*this)["if"] = token_id::REV_IF;
        (*this)["then"] = token_id::REV_THEN;
        (*this)["else"] = token_id::REV_ELSE;
        (*this)["while"] = token_id::REV_WHILE;
        (*this)["do"] = token_id::REV_DO;
        (*this)["for"] = token_id::REV_FOR;
    }
};

class operators_dict : public dictionary
{
public:
    operators_dict()
    {
        (*this)["+"] = token_id::OP_ADD;
        (*this)["*"] = token_id::OP_MUL;
        (*this)["-"] = token_id::OP_SUB;
        (*this)["/"] = token_id::OP_DIV;
        (*this)["("] = token_id::OP_LBRAC;
        (*this)[")"] = token_id::OP_RBRAC;
        (*this)["."] = token_id::OP_DOT;
        (*this)["="] = token_id::OP_ASSIGN;
        (*this)[">"] = token_id::OP_GT;
        (*this)["<"] = token_id::OP_LT;
        (*this)["=="] = token_id::OP_EQ;
        (*this)[">="] = token_id::OP_GE;
        (*this)["<="] = token_id::OP_LE;
        (*this)["!="] = token_id::OP_NE;
        (*this)["&"] = token_id::OP_ADDR;
        (*this)["["] = token_id::OP_LSBRAC;
        (*this)["]"] = token_id::OP_RSBRAC;
    }
};

class delimiter_dict : public dictionary
{
public:
    delimiter_dict()
    {
        (*this)[","] = token_id::DELIM_COMMA;
        (*this)[";"] = token_id::DELIM_SEMI;
        (*this)["//"] = token_id::DELIM_COMMENT_INL;
        (*this)["/*"] = token_id::DELIM_COMMENT_BEG;
        (*this)["*/"] = token_id::DELIM_COMMENT_END;
    }
};

class escape_dict : public unordered_map<string, char>
{
public:
    escape_dict()
    {
        (*this)["t"] = '\t';
        (*this)["n"] = '\n';
        (*this)["r"] = '\r';
        (*this)["\\"] = '\\';
        (*this)["\""] = '"';
    }
};

class linefeed_dict : public unordered_map<linefeed_type, string, hash_linefeed_type>
{
public:
    linefeed_dict()
    {
        (*this)[linefeed_type::OldMac] = "\r";
        (*this)[linefeed_type::Unix] = "\n";
        (*this)[linefeed_type::Windows] = "\r\n";
    }
};

#endif // CONSTANTS_H