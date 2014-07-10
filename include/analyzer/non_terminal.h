#ifndef NON_TERMINAL_H
#define NON_TERMINAL_H

#include <string>

#include "lex/token.h"
using std::string;

class analyze_context;
class decl_list;
class tkstream;

class non_terminal
{
public:
    virtual ~non_terminal() {}
    
    /**
     * Parse using this non_terminal
     * @param input the tokenstream of the input file.
     * @param context the compiler context.
     * @return true on succeed.
     */
    virtual bool parse(tkstream& input, analyze_context& context);
    
    /**
     * Whether the token is in this non_terminal's FIRST collection.
     * @return true if the cur_tk is found in the FIRST collection, otherwise false.
     */
    virtual bool can_accept(token cur_tk) = 0;
    
    /**
     * Whether this non_terminal accept empty string.
     * Default is false.
     * @return true if accept otherwise false.
     */
    virtual bool accept_empty();
};

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
};

class block_st : public non_terminal
{
public:
    virtual ~block_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
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
};

class else_st : public non_terminal
{
public:
    virtual ~else_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class while_st : public non_terminal
{
public:
    virtual ~while_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class do_while_st : public non_terminal
{
public:
    virtual ~do_while_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class for_st : public non_terminal
{
public:
    virtual ~for_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class empty_st : public non_terminal
{
public:
    virtual ~empty_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_st : public non_terminal
{
public:
    virtual ~expr_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class decl_item : public non_terminal
{
public:
    virtual ~decl_item() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
private:
    token item_;
    bool is_ptr_;
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

class expr : public non_terminal
{
public:
    virtual ~expr() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_h : public non_terminal
{
public:
    virtual ~expr_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_lv1 : public non_terminal
{
public:
    virtual ~expr_lv1() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_lv1_h : public non_terminal
{
public:
    virtual ~expr_lv1_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_lv2 : public non_terminal
{
public:
    virtual ~expr_lv2() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_lv2_h : public non_terminal
{
public:
    virtual ~expr_lv2_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_lv3 : public non_terminal
{
public:
    virtual ~expr_lv3() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class expr_item : public non_terminal
{
public:
    virtual ~expr_item() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr : public non_terminal
{
public:
    virtual ~bool_expr() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_h : public non_terminal
{
public:
    virtual ~bool_expr_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_lv1 : public non_terminal
{
public:
    virtual ~bool_expr_lv1() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_lv1_h : public non_terminal
{
public:
    virtual ~bool_expr_lv1_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_lv2 : public non_terminal
{
public:
    virtual ~bool_expr_lv2() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_lv2_h : public non_terminal
{
public:
    virtual ~bool_expr_lv2_h() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

class bool_expr_lv3 : public non_terminal
{
public:
    virtual ~bool_expr_lv3() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
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
    virtual ~decl_st_part() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    expr* first_expr() { return fst_exp_; }
    decl_list* item_list() { return list_; }
    
private:
    expr *fst_exp_;
    decl_list *list_;
};

class ret_st : public non_terminal
{
public:
    virtual ~ret_st() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

/*
 * Some optional non_terminal classes
 */ 
#include "analyzer/opt_base.h"
using opt_expr = opt_base< expr >;
using opt_starter = opt_base< starter >;
using opt_else_st = opt_base< else_st >;
#endif