#ifndef NON_TERMINAL_H
#define NON_TERMINAL_H

#include <string>
#include <vector>

#include "analyzer/non_terminal_base.h"
#include "lex/token.h"

using std::string;
using std::vector;

class expr;
class decl_list;

class single_token : public non_terminal
{
public:
    virtual ~single_token() {}
    single_token(token_id tkid);
    virtual bool parse(tkstream& input, analyze_context& context);
    bool can_accept(token cur_tk);
private:
    token tk_;
};

class epsilon : public non_terminal
{
public:
    virtual ~epsilon() {}
    virtual bool parse(tkstream& input, analyze_context& context) { return true; }
    bool can_accept(token cur_tk) { return true; }
};

/*
 * Classes derived from non_terminal
 */

class starter : public non_terminal
{
public:
    virtual ~starter() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class statement : public non_terminal
{
public:
    virtual ~statement() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class block_st : public non_terminal
{
public:
    virtual ~block_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class decl_st : public non_terminal
{
public:
    virtual ~decl_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class if_st : public non_terminal
{
public:
    virtual ~if_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class else_st : public non_terminal
{
public:
    virtual ~else_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class while_st : public non_terminal
{
public:
    virtual ~while_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class do_while_st : public non_terminal
{
public:
    virtual ~do_while_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class for_st : public non_terminal
{
public:
    virtual ~for_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class empty_st : public non_terminal
{
public:
    virtual ~empty_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class expr_st : public non_terminal
{
public:
    virtual ~expr_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class type_name : public non_terminal
{
public:
    virtual ~type_name() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string type_string();
private:
    token type_;
    bool is_ptr_;
};

class place_holder : public non_terminal
{
public:
    virtual ~place_holder() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    int address() const;
    void address(int new_addr) { address_ = new_addr; }
private:
    int address_;
};

class place_holder2 : public non_terminal
{
public:
    virtual ~place_holder2() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    int address() const;
    void address(int new_addr) { address_ = new_addr; }
private:
    int address_;
};

class starter_part : public non_terminal
{
public:
    virtual ~starter_part() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class decl_st_part : public non_terminal
{
public:
    decl_st_part(token name, const string& type)
        :name_tk_(name), name_(name.text()), type_(type) { }
    virtual ~decl_st_part() { }
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
private:
    token name_tk_;
    string name_;
    string type_;
};

class ret_st : public non_terminal
{
public:
    virtual ~ret_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class statements : public non_terminal
{
public:
    virtual ~statements() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    vector<int>& nextlist() { return nextlist_; }
private:
    vector<int> nextlist_;
};

class decl_item : public non_terminal
{
public:
    decl_item(const string& base_type) : base_type_(base_type) { }
    virtual ~decl_item() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string name() const { return item_.text(); }
    expr* init_expr() const { return init_expr_; }
private:
    token item_;
    expr* init_expr_;
    string base_type_;
};

/*
 * Some optional non_terminal classes
 */ 
#include "analyzer/opt_base.h"
using opt_expr = opt_base< expr >;
using opt_starter = opt_base< starter >;
using opt_else_st = opt_base< else_st >;
#endif