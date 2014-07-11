#ifndef TOKEN_H
#define TOKEN_H

#include <functional>
#include <string>
#include <unordered_map>

#include "lex/constants.h"
#include "lex/textpointer.h"

using std::string;

class token
{
public:
    token();
    
    token_id id() const;
    void id(const token_id &tid);
    
    text_pointer position() const;
    void position(const text_pointer &pointer);
    
    const string& text() const;
    void text(const string& str);
    
    bool good() const;
    
    bool isreserved() const;
    bool isidentifier() const;
    bool iscontant() const;
    bool isoperator() const;
    bool isdelimiter() const;
    bool iscomment() const;
    
private:
    token_id _id;
    text_pointer _pos;
    string _text;
    
public:
    static bool isspace(int ch);
    static bool isalnum(int ch);
    static bool isalpha(int ch);
    static bool isdigit(int ch);
    static bool isopfront(int ch);
    static bool isdlfront(int ch);
    static bool isquote(int ch);
    static bool issquote(int ch);
    static bool isdot(int ch);
    static bool isescapse(int ch);
    static bool isslash(int ch);
    static bool isstar(int ch);
    static bool islinefeedchar(int ch);
    static bool iseof(int ch);
    
    static bool isreserved(string str);
    static bool isoperator(string str);
    static bool isdelimiter(string str);
    static bool islinefeed(string str);
    
    static token_id id_reserved(string str);
    static token_id id_operator(string str);
    static token_id id_delimiter(string str);
    
    /**
     * @Return the corresponding escaped char.
     * i.e. escape('t') returns '\t'
     */
    static int escape(int ch);
    
    /**
     * Set the linefeed type.
     * Unix: \\n
     * Windows: \\r\\n
     * OldMac: \\r
     */
    static void set_linefeed(linefeed_type type);
    static string curr_linefeed();
    
    static const dictionary& reserved();
    static const dictionary& operators();
    static const dictionary& delimiter();
    
private:
    
    static reserved_dict _reserved;
    static operators_dict _operators;
    static delimiter_dict _delimiter;
    static escape_dict _escape;
    
    static linefeed_dict _linefeed;
    static linefeed_type _lftype;
};

#endif // TOKEN_H