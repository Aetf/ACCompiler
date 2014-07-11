#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>

#include "analyzer/non_terminal.h"

using std::string;
using std::vector;

class expr : public non_terminal
{
public:
    virtual ~expr() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string place() const { return place_; }
    string type() const { return type_; }
private:
    string place_;
    string type_;
};

class expr_lv1 : public non_terminal
{
public:
    virtual ~expr_lv1() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string place() const { return place_; }
    string type() const { return type_; }
private:
    string place_;
    string type_;
};

class expr_lv2 : public non_terminal
{
public:
    virtual ~expr_lv2() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string place() const { return place_; }
    string type() const { return type_; }
private:
    string place_;
    string type_;
};

class expr_lv3 : public non_terminal
{
public:
    virtual ~expr_lv3() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string place() const { return place_; }
    string type() const { return type_; }
private:
    string place_;
    string type_;
};

class expr_item : public non_terminal
{
public:
    virtual ~expr_item() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    string place() const { return place_; }
    string type() const { return type_; }
private:
    string place_;
    string type_;
};

class expr_h : public non_terminal
{
public:
    virtual ~expr_h();
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    token op_token() const { return op_token_; }
    expr_h* next() const { return nxt_h_; }
    expr_lv1* rh_expr() { return rh_expr_; }
    
private:
    token op_token_;
    expr_lv1* rh_expr_;
    expr_h* nxt_h_;
};

class expr_lv1_h : public non_terminal
{
public:
    virtual ~expr_lv1_h();
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    token op_token() const { return op_token_; }
    expr_lv1_h* next() const { return nxt_h_; }
    expr_lv2* rh_expr() { return rh_expr_; }
    
private:
    token op_token_;
    expr_lv2* rh_expr_;
    expr_lv1_h* nxt_h_;
};

class expr_lv2_h : public non_terminal
{
public:
    virtual ~expr_lv2_h();
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    token op_token() const { return op_token_; }
    expr_lv2_h* next() const { return nxt_h_; }
    expr_lv3* rh_expr() { return rh_expr_; }
    
private:
    token op_token_;
    expr_lv3* rh_expr_;
    expr_lv2_h* nxt_h_;
};

class bool_expr : public non_terminal
{
public:
    virtual ~bool_expr() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& true_list() { return true_list_; }
    vector<int>& false_list() { return false_list_; }
private:
    vector<int> true_list_;
    vector<int> false_list_;
};

class bool_expr_lv1 : public non_terminal
{
public:
    virtual ~bool_expr_lv1() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& true_list() { return true_list_; }
    vector<int>& false_list() { return false_list_; }
private:
    vector<int> true_list_;
    vector<int> false_list_;
};

class bool_expr_lv3 : public non_terminal
{
public:
    virtual ~bool_expr_lv3() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& true_list() { return true_list_; }
    vector<int>& false_list() { return false_list_; }
private:
    vector<int> true_list_;
    vector<int> false_list_;
};

class bool_expr_lv2 : public non_terminal
{
public:
    virtual ~bool_expr_lv2() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    vector<int>& true_list() { return true_list_; }
    vector<int>& false_list() { return false_list_; }
private:
    vector<int> true_list_;
    vector<int> false_list_;
};

class bool_expr_h : public non_terminal
{
public:
    virtual ~bool_expr_h();
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    token op_token() const { return op_token_; }
    bool_expr_h* next() const { return nxt_h_; }
    bool_expr_lv1* rh_expr() { return rh_expr_; }
    int rh_entry() const {return rh_entry_; }
    
private:
    token op_token_;
    bool_expr_h* nxt_h_;
    bool_expr_lv1* rh_expr_;
    int rh_entry_;
};

class bool_expr_lv1_h : public non_terminal
{
public:
    virtual ~bool_expr_lv1_h();
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    token op_token() const { return op_token_; }
    bool_expr_lv1_h* next() const { return nxt_h_; }
    bool_expr_lv3* rh_expr() { return rh_expr_; }
    int rh_entry() const {return rh_entry_; }
    
private:
    token op_token_;
    bool_expr_lv1_h* nxt_h_;
    bool_expr_lv3* rh_expr_;
    int rh_entry_;
};



#endif // EXPRESSION_H