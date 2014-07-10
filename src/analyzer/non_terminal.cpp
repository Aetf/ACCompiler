#include "analyzer/analyze_context.h"
#include "analyzer/func_base.h"
#include "analyzer/list_base.h"
#include "analyzer/non_terminal.h"
#include "lex/constants.h"
#include "lex/tkstream.h"
#include "lex/token.h"

#include "analyzer/helpers.h"

bool non_terminal::parse(tkstream& input, analyze_context& context)
{
    if (!input.peek().good() && !accept_empty()) {
        context.on_error(input.peek());
        return false;
    }
    
    if (!can_accept(input.peek())) {
        context.on_error(input.peek());
        return false;
    }
    return true;
}

bool non_terminal::accept_empty()
{
    return false;
}

single_token::single_token(token_id tkid)
{
    tk_.id(tkid);
}

bool single_token::can_accept(token cur_tk)
{
    return cur_tk.id() == tk_.id();
}

bool single_token::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    extract_to(tk_.id(), tk_);

exit:
    return res;
}

bool starter::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    starter_part *part = new starter_part;
    opt_starter *more = new opt_starter;
    
    parse_use(part);
    parse_use(more);
    
exit:
    if (nullptr != part) delete part;
    if (nullptr != more) delete more;
    return res;
}

bool starter::can_accept(token cur_tk)
{
    type_name tp;
    return tp.can_accept(cur_tk);
}

bool statement::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool         res     = true;
    decl_st     *decl    = new decl_st;
    block_st    *block   = new block_st;
    if_st       *ifst    = new if_st;
    while_st    *whilest = new while_st;
    do_while_st *dowh    = new do_while_st;
    for_st      *forst   = new for_st;
    empty_st    *empty   = new empty_st;
    expr_st     *exprst  = new expr_st;
    if (decl->can_accept(input.peek())) {
        if (!decl->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (block->can_accept(input.peek())) {
        if (!block->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (ifst->can_accept(input.peek())) {
        if (!ifst->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (whilest->can_accept(input.peek())) {
        if (!whilest->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (dowh->can_accept(input.peek())) {
        if (!dowh->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (forst->can_accept(input.peek())) {
        if (!forst->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (empty->can_accept(input.peek())) {
        if (!empty->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (exprst->can_accept(input.peek())) {
        if (!exprst->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }

exit:
    if (nullptr != decl) delete decl;
    if (nullptr != block) delete block;
    if (nullptr != ifst) delete ifst;
    if (nullptr != whilest) delete whilest;
    if (nullptr != dowh) delete dowh;
    if (nullptr != forst) delete forst;
    if (nullptr != empty) delete empty;
    if (nullptr != exprst) delete exprst;
    return res;
}

bool statement::can_accept(token cur_tk)
{
    decl_st     decl;
    block_st    block;
    if_st       ifst;
    while_st    whilest;
    do_while_st dowh;
    for_st      forst;
    empty_st    empty;
    expr_st     exprst;
    return decl.can_accept(cur_tk)
        || block.can_accept(cur_tk)
        || ifst.can_accept(cur_tk)
        || whilest.can_accept(cur_tk)
        || dowh.can_accept(cur_tk)
        || forst.can_accept(cur_tk)
        || empty.can_accept(cur_tk)
        || exprst.can_accept(cur_tk);
}

bool block_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    opt_decl_sts *decls = new opt_decl_sts;
    
    advance_if(token_id::REV_BEGIN);
    parse_use(decls);
    advance_if(token_id::REV_END);

exit:
    if (nullptr != decls) delete decls;
    return res;
}

bool block_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_BEGIN;
}

bool decl_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    type_name *type = new type_name;
    decl_list *list = new decl_list;
    
    parse_use(type);
    parse_use(list);
    advance_if(token_id::DELIM_SEMI);
    
    // TODO: Add list items to symbol table
exit:
    if (nullptr != type) delete type;
    if (nullptr != list) delete list;
    return res;
}

bool decl_st::can_accept(token cur_tk)
{
    type_name type;
    return type.can_accept(cur_tk);
}

bool if_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr *exp = new bool_expr;
    place_holder *ph = new place_holder;
    statement *st = new statement;
    place_holder2 *ph2 = new place_holder2;
    opt_else_st *optel = new opt_else_st;
    
    advance_if(token_id::REV_IF);
    advance_if(token_id::OP_LBRAC);
    parse_use(exp);
    advance_if(token_id::OP_RBRAC);
    parse_use(ph);
    parse_use(st);
    parse_use(ph2);
    parse_use(optel);
    
    // TODO: if semantic
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != ph) delete ph;
    if (nullptr != st) delete st;
    if (nullptr != ph2) delete ph2;
    if (nullptr != optel) delete optel;
    return res;
}

bool if_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_IF;
}

bool else_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    place_holder *ph = new place_holder;
    statement *st = new statement;
    
    advance_if(token_id::REV_ELSE);
    parse_use(ph);
    parse_use(st);
    
    // TODO: if semantic
    
exit:
    if (nullptr != ph) delete ph;
    if (nullptr != st) delete st;
    return res;
}

bool else_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_ELSE;
}

bool while_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr *exp = new bool_expr;
    place_holder *ph = new place_holder;
    place_holder *phh = new place_holder;
    statement *st = new statement;
    
    advance_if(token_id::REV_WHILE);
    advance_if(token_id::OP_LBRAC);
    parse_use(ph);
    parse_use(exp);
    advance_if(token_id::OP_RBRAC);
    parse_use(phh);
    parse_use(st);
    
    // TODO: while semantic
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != ph) delete ph;
    if (nullptr != phh) delete phh;
    if (nullptr != st) delete st;
    return res;
}

bool while_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_WHILE;
}

bool do_while_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    place_holder *ph = new place_holder;
    statement *st = new statement;
    bool_expr *exp = new bool_expr;
    
    advance_if(token_id::REV_DO);
    parse_use(ph);
    parse_use(st);
    advance_if(token_id::REV_WHILE);
    advance_if(token_id::OP_LBRAC);
    parse_use(exp);
    advance_if(token_id::OP_RBRAC);
    advance_if(token_id::DELIM_SEMI);
    
    // TODO: do while semantic
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != ph) delete ph;
    if (nullptr != st) delete st;
    return res;
}

bool do_while_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_DO;
}

bool for_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr *initexp = new expr;
    place_holder *ph = new place_holder;
    bool_expr *boolexp = new bool_expr;
    statement *st = new statement;
    expr *stepexp = new expr;
    
    advance_if(token_id::REV_FOR);
    advance_if(token_id::OP_LBRAC);
    parse_use(initexp);
    advance_if(token_id::DELIM_SEMI);
    parse_use(ph);
    parse_use(boolexp);
    advance_if(token_id::DELIM_SEMI);
    parse_use(stepexp);
    advance_if(token_id::OP_RBRAC);
    parse_use(st);
    
    // TODO: for semantic
    
exit:
    if (nullptr != initexp) delete initexp;
    if (nullptr != ph) delete ph;
    if (nullptr != boolexp) delete boolexp;
    if (nullptr != st) delete st;
    if (nullptr != stepexp) delete stepexp;
    return res;
}

bool for_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_FOR;
}

bool empty_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    advance_if(token_id::DELIM_SEMI);
    
exit:
    return res;
}

bool empty_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::DELIM_SEMI;
}

bool expr_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr *exp = new expr;
    
    parse_use(exp);
    advance_if(token_id::DELIM_SEMI);
    
exit:
    if (nullptr != exp) delete exp;
    return res;
}

bool expr_st::can_accept(token cur_tk)
{
    expr exp;
    return exp.can_accept(cur_tk);
}

bool decl_item::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr *exp = new expr;
    
    is_ptr_ = false;
    if (input.peek().id() == token_id::OP_MUL) {
        input.advance();
        is_ptr_ = true;
    }
    
    extract_to(token_id::IDENTIFIER, item_);
    // TODO: assign symbol table entry
    
    // optional assignment expression
    if (input.peek().id() == token_id::OP_ASSIGN) {
        input.advance();
        parse_use(exp);
        
        // TODO: assignment expression semantic
    }
    
exit:
    if (nullptr != exp) delete exp;
    return res;
}

bool decl_item::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::IDENTIFIER
        || cur_tk.id() == token_id::OP_MUL;
}

bool type_name::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    is_ptr_ = false;
    if (!this->can_accept(input.peek())) {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    input >> type_;
    
    if (input.peek().id() == token_id::OP_MUL) {
        input.advance();
        is_ptr_ = true;
    }
    
exit:
    return res;
}

bool type_name::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_INT
        || cur_tk.id() == token_id::REV_FLOAT
        || cur_tk.id() == token_id::REV_CHAR;
}

string type_name::type_string()
{
    string str;
    switch (type_.id())
    {
    case token_id::REV_INT: str = "int"; break;
    case token_id::REV_CHAR: str = "char"; break;
    case token_id::REV_FLOAT: str = "float"; break;
    default: str = "unknown"; break;
    }
    
    if (is_ptr_) str += "*";
    return str;
}

bool place_holder::parse(tkstream& input, analyze_context& context)
{
    address(context.next_address());
    return true;
}

bool place_holder::can_accept(token cur_tk)
{
    return true;
}

bool place_holder2::parse(tkstream& input, analyze_context& context)
{
    address(context.next_address());
    return true;
}

bool place_holder2::can_accept(token cur_tk)
{
    return true;
}

bool expr::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    expr_lv1 *lv1 = new expr_lv1;
    expr_h *h = new expr_h;
    parse_use(lv1);
    parse_use(h);
    
exit:
    if (nullptr != lv1) delete lv1;
    if (nullptr != h) delete h;
    return res;
}

bool expr::can_accept(token cur_tk)
{
    expr_lv1 lv1;
    return lv1.can_accept(cur_tk);
}

bool expr_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr_lv1 *lv1 = new expr_lv1;
    expr_h *h = new expr_h;
    
    if (input.peek().id() == token_id::OP_ADD) {
        input.advance();
        parse_use(lv1);
        parse_use(h);
        // TODO
    } else if (input.peek().id() == token_id::OP_SUB) {
        input.advance();
        parse_use(lv1);
        parse_use(h);
        // TODO
    }
    
exit:
    if (nullptr != lv1) delete lv1;
    if (nullptr != h) delete h;
    return res;
}

bool expr_h::can_accept(token cur_tk)
{
    return true;
}

bool expr_lv1::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    expr_lv2 *lv2 = new expr_lv2;
    expr_lv1_h *h = new expr_lv1_h;
    parse_use(lv2);
    parse_use(h);
    
exit:
    if (nullptr != lv2) delete lv2;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv1::can_accept(token cur_tk)
{
    expr_lv2 lv2;
    return lv2.can_accept(cur_tk);
}

bool expr_lv1_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr_lv2 *lv2 = new expr_lv2;
    expr_lv1_h *h = new expr_lv1_h;
    
    if (input.peek().id() == token_id::OP_MUL) {
        input.advance();
        parse_use(lv2);
        parse_use(h);
        // TODO
    } else if (input.peek().id() == token_id::OP_DIV) {
        input.advance();
        parse_use(lv2);
        parse_use(h);
        // TODO
    }
    
    exit:
    if (nullptr != lv2) delete lv2;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv1_h::can_accept(token cur_tk)
{
    return true;
}

bool expr_lv2::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    expr_lv3 *lv3 = new expr_lv3;
    expr_lv2_h *h = new expr_lv2_h;
    parse_use(lv3);
    parse_use(h);
    
exit:
    if (nullptr != lv3) delete lv3;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv2::can_accept(token cur_tk)
{
    expr_lv3 lv3;
    return lv3.can_accept(cur_tk);
}

bool expr_lv2_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr_lv3 *lv3 = nullptr;
    expr_lv2_h *h = nullptr;
    token op;
    if (input.peek().id() == token_id::OP_ADDR) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_SUB) {
        input >> op;
    } else if (input.peek().id() == token_id::REV_NOT) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_MUL) {
        input >> op;
    }
    
    if (op.good()) {
        lv3 = new expr_lv3;
        h = new expr_lv2_h;
        parse_use(lv3);
        parse_use(h);
        
        // TODO semantic
    }
    
exit:
    if (nullptr != lv3) delete lv3;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv2_h::can_accept(token cur_tk)
{
    return true;
}

bool expr_lv3::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr *exp = new expr;
    expr_item *item = new expr_item;
    if (input.peek().id() == token_id::OP_LBRAC) {
        input.advance();
        
        parse_use(exp);
        advance_if(token_id::OP_RBRAC);
        
        // TODO
    } else if (item->can_accept(input.peek())) {
        if (!item->parse(input, context)) {
            res = false;
            goto exit;
        }
        // TODO
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    // TODO: semantic
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != item) delete item;
    return res;
}

bool expr_lv3::can_accept(token cur_tk)
{
    expr_item item;
    return cur_tk.id() == token_id::OP_LBRAC
        || item.can_accept(cur_tk);
}

bool expr_item::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    opt_aparams *ap = new opt_aparams;
    array_dim *ad = new array_dim;
    
    token id;
    extract_to(token_id::IDENTIFIER, id);
    
    if (input.peek().id() == token_id::OP_LBRAC) {
        parse_use(ap);
        // TODO: semantic
    } else if (input.peek().id() == token_id::OP_LSBRAC) {
        parse_use(ad);
        // TODO: semantic
    }
    
    // TODO: semantic
    
exit:
    if (nullptr != ap) delete ap;
    if (nullptr != ad) delete ad;
    return res;
}

bool expr_item::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::IDENTIFIER;
}

bool bool_expr::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    bool_expr_lv1 *lv1 = new bool_expr_lv1;
    bool_expr_h *h = new bool_expr_h;
    parse_use(lv1);
    parse_use(h);
    
exit:
    if (nullptr != lv1) delete lv1;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr::can_accept(token cur_tk)
{
    bool_expr_lv1 lv1;
    return lv1.can_accept(cur_tk);
}

bool bool_expr_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr_lv1 *lv1 = new bool_expr_lv1;
    bool_expr_h *h = new bool_expr_h;
    
    if (input.peek().id() == token_id::REV_AND) {
        input.advance();
        parse_use(lv1);
        parse_use(h);
        // TODO
    }
    
exit:
    if (nullptr != lv1) delete lv1;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_h::can_accept(token cur_tk)
{
    return true;
}

bool bool_expr_lv1::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    bool_expr_lv2 *lv2 = new bool_expr_lv2;
    bool_expr_lv1_h *h = new bool_expr_lv1_h;
    parse_use(lv2);
    parse_use(h);
    
    exit:
    if (nullptr != lv2) delete lv2;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv1::can_accept(token cur_tk)
{
    bool_expr_lv2 lv2;
    return lv2.can_accept(cur_tk);
}

bool bool_expr_lv1_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr_lv2 *lv2 = new bool_expr_lv2;
    bool_expr_lv1_h *h = new bool_expr_lv1_h;
    
    if (input.peek().id() == token_id::REV_OR) {
        input.advance();
        parse_use(lv2);
        parse_use(h);
        // TODO
    }
    
exit:
    if (nullptr != lv2) delete lv2;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv1_h::can_accept(token cur_tk)
{
    return true;
}

bool bool_expr_lv2::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    bool_expr_lv3 *lv3 = new bool_expr_lv3;
    bool_expr_lv2_h *h = new bool_expr_lv2_h;
    parse_use(lv3);
    parse_use(h);
    
exit:
    if (nullptr != lv3) delete lv3;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv2::can_accept(token cur_tk)
{
    bool_expr_lv3 lv3;
    return lv3.can_accept(cur_tk);
}

bool bool_expr_lv2_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr_lv3 *lv3 = new bool_expr_lv3;
    bool_expr_lv2_h *h = new bool_expr_lv2_h;
    token op;
    if (input.peek().id() == token_id::OP_EQ) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_NE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_GE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_LE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_GT) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_LT) {
        input >> op;
    }
    
    if (op.good()) {
        parse_use(lv3);
        parse_use(h);
        // TODO
    }
    
exit:
    if (nullptr != lv3) delete lv3;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv2_h::can_accept(token cur_tk)
{
    return true;
}

bool bool_expr_lv3::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    expr *exp = new expr;
    bool_expr *boolexp = new bool_expr;
    if (input.peek().id() == token_id::OP_LBRAC) {
        input.advance();
        parse_use(boolexp);
        advance_if(token_id::OP_RBRAC);
        
        // TODO
    } else if (exp->can_accept(input.peek())) {
        if (!exp->parse(input, context)) {
            res = false;
            goto exit;
        }
        // TODO
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    // TODO: semantic
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != boolexp) delete boolexp;
    return res;
}

bool bool_expr_lv3::can_accept(token cur_tk)
{
    expr exp;
    return cur_tk.id() == token_id::OP_LBRAC
        || exp.can_accept(cur_tk);
}

bool starter_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool type_has_star = false;
    token idtk;
    
    type_name *tp = new type_name;
    decl_st_part *decl = new decl_st_part;
    func_def_part *fdef = new func_def_part;
    
    parse_use(tp);
    
    if (input.peek().id() == token_id::OP_MUL) {
        type_has_star = true;
        input.advance();
    }
    
    extract_to(token_id::IDENTIFIER, idtk);
    
    if (decl->can_accept(input.peek())) {
        if (!decl->parse(input, context)) {
            res = false;
            goto exit;
        }
        // TODO: decl_st semantic
    } else if (fdef->can_accept(input.peek())) {
        if (!fdef->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        // TODO: func_def semantic
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    
exit:
    if (nullptr != tp) delete tp;
    if (nullptr != decl) delete decl;
    if (nullptr != fdef) delete fdef;
    return res;
}

bool starter_part::can_accept(token cur_tk)
{
    type_name tp;
    return tp.can_accept(cur_tk);
}

bool decl_st_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    list_ = nullptr;
    fst_exp_ = nullptr;
    
    if (input.peek().id() == token_id::OP_ASSIGN) {
        input.advance();
        fst_exp_ = new expr;
        
        parse_use(fst_exp_);
    }
    
    if (input.peek().id() == token_id::DELIM_COMMA) {
        list_ = new decl_list;
        
        input.advance();
        parse_use(list_);
        advance_if(token_id::DELIM_SEMI);
    }    
    
    
    // defered decl semantic in starter_part.
exit:
    return res;
}

bool decl_st_part::can_accept(token cur_tk)
{
    return true;
}

bool ret_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    opt_expr *oexp = new opt_expr;
    
    advance_if(token_id::REV_RETURN);
    
    parse_use(oexp);
    
    advance_if(token_id::DELIM_SEMI);

    // function return
exit:
    if (nullptr != oexp) delete oexp;
    return res;
}

bool ret_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_RETURN;
}
