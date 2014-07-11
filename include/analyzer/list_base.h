#ifndef LIST_BASE_H
#define LIST_BASE_H

#include <vector>
#include "analyzer/func_base.h"
#include "analyzer/non_terminal.h"
#include "analyzer/symbol_table.h"

using std::vector;

template <typename Item_Type>
class list_more_base;

/**
 * Base class for all list related non_terminals
 */
template <typename Item_Type>
class list_base : public non_terminal
{
public:
    virtual ~list_base() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    
    /**
     * All list item in this list
     * Note: this content is filled only after the parse() method is succeed.
     */
    vector<Item_Type>& items() { return items_; }
    
protected:
    /**
     * Parse the single part of a list
     */
    virtual bool parse_single(tkstream& input, analyze_context& context) = 0;
    
    /**
     * Create a non_terminal to parse the list_more part
     * @return a new allocated non_terminal
     */
    virtual list_more_base<Item_Type>* create_list_more() = 0;
    
private:
    vector<Item_Type> items_;
};

template <typename Item_Type>
class list_more_base : public non_terminal
{
public:
    virtual ~list_more_base() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool accept_empty() override;
    virtual bool can_accept(token cur_tk) override;
    
    /**
     * All list item in this list
     * Note: this content is filled only after the parse() method is succeed.
     */
    vector<Item_Type>& items() { return items_; }
protected:
    /**
     * Create a non_terminal to parse the seperator part
     * @return a new allocated non_terminal
     */
    virtual non_terminal* create_sep() = 0;
    
    /**
     * Create a non_terminal to parse the list part
     * @return a new allocated non_terminal
     */
    virtual list_base<Item_Type>* create_list() = 0;
    
private:
    vector<Item_Type> items_;
};


/*
 * Classes derived from list_base and list_more_base
 */
class fparams : public list_base<func_arg>
{
public:
    virtual ~fparams() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<func_arg>* create_list_more() override;
};

class fparams_more : public list_more_base<func_arg>
{
public:
    virtual ~fparams_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<func_arg>* create_list() override;
};

class aparams : public list_base<expr>
{
public:
    virtual ~aparams() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<expr>* create_list_more() override;
};

class aparams_more : public list_more_base<expr>
{
public:
    virtual ~aparams_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<expr>* create_list() override;
};

class decl_list : public list_base<decl_item>
{
public:
    decl_list(const string& base_type) :base_type_(base_type) { }
    virtual ~decl_list() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<decl_item>* create_list_more() override;
private:
    string base_type_;
};

class decl_list_more : public list_more_base<decl_item>
{
public:
    decl_list_more(const string& base_type) :base_type_(base_type) { }
    virtual ~decl_list_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<decl_item>* create_list() override;
private:
    string base_type_;
};

class array_dim : public list_base<expr>
{
public:
    virtual ~array_dim() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<expr>* create_list_more() override;
};

class array_dim_more : public list_more_base<expr>
{
public:
    virtual ~array_dim_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<expr>* create_list() override;
};

class int_list : public list_base<int>
{
public:
    virtual ~int_list() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<int>* create_list_more() override;
};

class int_list_more : public list_more_base<int>
{
public:
    virtual ~int_list_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<int>* create_list() override;
};

class decl_sts : public list_base<decl_st>
{
public:
    virtual ~decl_sts() {}
    virtual bool can_accept(token cur_tk) override;
protected:
    virtual bool parse_single(tkstream& input, analyze_context& context) override;
    virtual list_more_base<decl_st>* create_list_more() override;
};

class decl_sts_more : public list_more_base<decl_st>
{
public:
    virtual ~decl_sts_more() {}
protected:
    virtual non_terminal* create_sep() override;
    virtual list_base<decl_st>* create_list() override;
};

/*
 * Some optional non_terminal classes
 */
#include "analyzer/opt_base.h"
using opt_fparams = opt_base< fparams >;
using opt_aparams = opt_base< aparams >;
using opt_decl_sts = opt_base< decl_sts >;
#endif