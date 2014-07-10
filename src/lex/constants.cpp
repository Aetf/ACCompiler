#include "lex/constants.h"

std::ostream& operator<<(std::ostream &os, const token_id &id)
{
    switch(id)
    {
        case token_id::ILLIGEL: return os << "ILLIGEL";
        case token_id::REV_PROGRAM: return os << "REV_PROGRAM";
        case token_id::REV_BEGIN: return os << "REV_BEGIN";
        case token_id::REV_END: return os << "REV_END";
        case token_id::REV_RETURN: return os << "REV_RETURN";
        case token_id::REV_INT: return os << "REV_INT";
        case token_id::REV_FLOAT: return os << "REV_FLOAT";
        case token_id::REV_CHAR: return os << "REV_CHAR";
        case token_id::REV_AND: return os << "REV_AND";
        case token_id::REV_OR: return os << "REV_OR";
        case token_id::REV_NOT: return os << "REV_NOT";
        case token_id::REV_IF: return os << "REV_IF";
        case token_id::REV_THEN: return os << "REV_THEN";
        case token_id::REV_ELSE: return os << "REV_ELSE";
        case token_id::REV_WHILE: return os << "REV_WHILE";
        case token_id::REV_DO: return os << "REV_DO";
        case token_id::OP_ADD: return os << "OP_ADD";
        case token_id::OP_MUL: return os << "OP_MUL";
        case token_id::OP_SUB: return os << "OP_SUB";
        case token_id::OP_DIV: return os << "OP_DIV";
        case token_id::OP_LBRAC: return os << "OP_LBRAC";
        case token_id::OP_RBRAC: return os << "OP_RBRAC";
        case token_id::OP_LSBRAC: return os << "OP_LSBRAC";
        case token_id::OP_RSBRAC: return os << "OP_RSBRAC";
        case token_id::OP_DOT: return os << "OP_DOT";
        case token_id::OP_ADDR: return os << "OP_ADDR";
        case token_id::OP_ASSIGN: return os << "OP_ASSIGN";
        case token_id::OP_GT: return os << "OP_GT";
        case token_id::OP_LT: return os << "OP_LT";
        case token_id::OP_EQ: return os << "OP_EQ";
        case token_id::OP_GE: return os << "OP_GE";
        case token_id::OP_LE: return os << "OP_LE";
        case token_id::OP_NE: return os << "OP_NE";
        case token_id::DELIM_COMMA: return os << "DELIM_COMMA";
        case token_id::DELIM_SEMI: return os << "DELIM_SEMI";
        case token_id::DELIM_COMMENT_INL: return os << "DELIM_COMMENT_INL";
        case token_id::DELIM_COMMENT_BEG: return os << "DELIM_COMMENT_BEG";
        case token_id::DELIM_COMMENT_END: return os << "DELIM_COMMENT_END";
        case token_id::CONST_INT: return os << "CONST_INT";
        case token_id::CONST_FLOAT: return os << "CONST_FLOAT";
        case token_id::CONST_STR: return os << "CONST_STR";
        case token_id::CONST_BOOL: return os << "CONST_BOOL";
        case token_id::COMMENT: return os << "COMMENT";
        case token_id::IDENTIFIER: return os << "IDENTIFIER";
        default: return os << "(invalid value)";
    }
}