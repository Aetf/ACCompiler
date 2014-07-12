#include "analyzer/analyze_context.h"
#include "analyzer/func_base.h"
#include "analyzer/list_base.h"
#include "analyzer/non_terminal.h"
#include "analyzer/expression.h"
#include "lex/constants.h"
#include "lex/tkstream.h"
#include "lex/token.h"

#include "analyzer/helpers.h"
#include <analyzer/acc_types.h>

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
    ret_st      *ret     = new ret_st;
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
        this->nextlist_ = block->nextlist();
    } else if (ifst->can_accept(input.peek())) {
        if (!ifst->parse(input, context)) {
            res = false;
            goto exit;
        }
        this->nextlist_ = ifst->nextlist();
    } else if (whilest->can_accept(input.peek())) {
        if (!whilest->parse(input, context)) {
            res = false;
            goto exit;
        }
        this->nextlist_ = whilest->nextlist();
    } else if (dowh->can_accept(input.peek())) {
        if (!dowh->parse(input, context)) {
            res = false;
            goto exit;
        }
        this->nextlist_ = dowh->nextlist();
    } else if (forst->can_accept(input.peek())) {
        if (!forst->parse(input, context)) {
            res = false;
            goto exit;
        }
        this->nextlist_ = forst->nextlist();
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
    } else if (ret->can_accept(input.peek())) {
        if (!ret->parse(input, context)) {
            res = false;
            goto exit;
        }
        this->nextlist_ = ret->nextlist();
    } else {
        context.on_error("expected statement here", input.peek().position());
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
    ret_st      ret;
    return decl.can_accept(cur_tk)
        || block.can_accept(cur_tk)
        || ifst.can_accept(cur_tk)
        || whilest.can_accept(cur_tk)
        || dowh.can_accept(cur_tk)
        || forst.can_accept(cur_tk)
        || empty.can_accept(cur_tk)
        || ret.can_accept(cur_tk)
        || exprst.can_accept(cur_tk);
}

bool block_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    opt_decl_sts *decls = new opt_decl_sts;
    statements *sts = new statements;
    
    advance_if(token_id::OP_LBRAC);
    parse_use(decls);
    parse_use(sts);
    advance_if(token_id::OP_RBRAC);
    
    this->nextlist_ = sts->nextlist();

exit:
    if (nullptr != decls) delete decls;
    if (nullptr != sts) delete sts;
    return res;
}

bool block_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::OP_LBRAC;
}

bool decl_st::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    type_name *type = new type_name;
    decl_list *list = nullptr;
    
    parse_use(type);
    
    list = new decl_list(type->type_string());
    parse_use(list);
    advance_if(token_id::DELIM_SEMI);
    
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
    statement *st = new statement;
    opt_else_st *optel = new opt_else_st;
    
    int trueaddr, falseaddr = -1;
    
    advance_if(token_id::REV_IF);
    advance_if(token_id::OP_LPAREN);
    parse_use(exp);
    advance_if(token_id::OP_RPAREN);

    trueaddr = context.next_address();
    parse_use(st);
    
    context.back_patch(exp->true_list(), trueaddr);
    this->nextlist().push_back(context.next_address());
    context.back_patch(st->nextlist(), 
                context.generate("jmp", "", "", ""));
    
    falseaddr = context.next_address();    
    parse_use(optel);
    
    if (!optel->empty()) {
        // else statement
        context.back_patch(exp->false_list(), falseaddr);
        
        this->nextlist().insert(this->nextlist().begin(),
                                optel->actual_nt()->nextlist().begin(),
                                optel->actual_nt()->nextlist().end());
    } else {
        this->nextlist().insert(this->nextlist().begin(),
                                exp->false_list().begin(),
                                exp->false_list().end());
    }
    
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != st) delete st;
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
    statement *st = new statement;
    
    advance_if(token_id::REV_ELSE);
    parse_use(st);
    
    this->nextlist_ = st->nextlist();
    
exit:
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
    statement *st = new statement;
    int beginaddr, staddr;
    
    advance_if(token_id::REV_WHILE);
    advance_if(token_id::OP_LPAREN);
    
    beginaddr = context.next_address();
    parse_use(exp);
    advance_if(token_id::OP_RPAREN);
    
    staddr = context.next_address();
    parse_use(st);
    
    {
        ostringstream oss;
        oss << beginaddr;
        context.back_patch(st->nextlist(),
                           context.generate("jmp", "", "", oss.str()));
        
        context.back_patch(exp->true_list(), staddr);
        this->nextlist_ = exp->false_list();
    }
    
exit:
    if (nullptr != exp) delete exp;
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
    statement *st = new statement;
    bool_expr *exp = new bool_expr;
    int staddr;
    
    advance_if(token_id::REV_DO);
    
    staddr = context.next_address();
    parse_use(st);
    advance_if(token_id::REV_WHILE);
    advance_if(token_id::OP_LPAREN);
    parse_use(exp);
    advance_if(token_id::OP_RPAREN);
    advance_if(token_id::DELIM_SEMI);
    
    context.back_patch(exp->true_list(), staddr);
    this->nextlist_ = exp->false_list();
    
exit:
    if (nullptr != exp) delete exp;
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
    bool_expr *boolexp = new bool_expr;
    statement *st = new statement;
    expr *stepexp = new expr;
    int beginaddr, staddr,stpaddr;
    ostringstream bss, sss, stpss;
    
    advance_if(token_id::REV_FOR);
    advance_if(token_id::OP_LPAREN);
    parse_use(initexp);
    advance_if(token_id::DELIM_SEMI);
    
    beginaddr = context.next_address();
    parse_use(boolexp);
    advance_if(token_id::DELIM_SEMI);
    
    stpaddr = context.next_address();
    parse_use(stepexp);
    bss << beginaddr;
    context.generate("jmp", "", "", bss.str());
    
    advance_if(token_id::OP_RPAREN);
    staddr = context.next_address();
    parse_use(st);
    context.back_patch(st->nextlist(), context.next_address());
    stpss << stpaddr;
    context.generate("jmp", "", "", stpss.str());
    
    // semantic
    context.back_patch(boolexp->true_list(), staddr);
    this->nextlist_ = boolexp->false_list();
exit:
    if (nullptr != initexp) delete initexp;
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
    init_expr_ = nullptr;
    int_list *dims = nullptr;
    
    if (input.peek().id() == token_id::OP_MUL) {
        input.advance();
        base_type_ += "*";
    }
    
    extract_to(token_id::IDENTIFIER, item_);
    
    // add to symbol table
    {
        symbol_entry entry;
        if (!context.table().new_variable(item_.text(), base_type_, entry)) {
            context.on_error("redefinition of " + item_.text(), item_.position());
            res = false;
            goto exit;
        }
        
        if (input.peek().id() == token_id::OP_ASSIGN) {
            // optional assignment expression
            input.advance();
            init_expr_ = new expr;
            parse_use(init_expr_);
            
            // mov expr result to variable
            context.generate("mov", init_expr_->place(), "", 
                             entry->second.address_str());
        } else if (input.peek().id() == token_id::OP_LSBRAC) {
            // optional array declaration
            input.advance();
            dims = new int_list;
            parse_use(dims);
            advance_if(token_id::OP_RSBRAC);
            
            string arr_type = context.table()
            .register_array(base_type_, dims->items());
            entry->second.type(arr_type);
        }
    }
    
exit:
    if (nullptr != dims) delete dims;
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
        context.on_error("expected type name here", input.peek().position());
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

bool starter_part::can_accept(token cur_tk)
{
    type_name tp;
    return tp.can_accept(cur_tk);
}

bool starter_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool type_has_star = false;
    token idtk;
    string type;
    
    type_name *tp = new type_name;
    decl_st_part *decl = nullptr;
    func_def_part *fdef = nullptr;
    
    parse_use(tp);
    
    if (input.peek().id() == token_id::OP_MUL) {
        type_has_star = true;
        input.advance();
    }
    
    extract_to(token_id::IDENTIFIER, idtk);
    
    type = tp->type_string();
    if (type_has_star) type += "*";
    
    decl = new decl_st_part(idtk, type);
    fdef = new func_def_part(idtk, type);
    
    if (decl->can_accept(input.peek())) {
        if (!decl->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else if (fdef->can_accept(input.peek())) {
        if (!fdef->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else {
        context.on_error("expected variable declaration or function declaration", input.peek().position());
        res = false;
        goto exit;
    }
    
exit:
    
    if (nullptr != tp) delete tp;
    if (nullptr != decl) delete decl;
    if (nullptr != fdef) delete fdef;
    return res;
}

bool decl_st_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    decl_list *list_ = nullptr;
    expr *fst_exp_ = nullptr;
    int_list *dims = nullptr;
    
    
    // add the first to symbol table
    if (!context.table().new_variable(name_, type_)) {
        context.on_error("redefinition of " + name_tk_.text(), name_tk_.position());
        res = false;
        goto exit;
    }
    
    // with optional init expression
    {
        variable_desc desc;
        context.entry_of(name_, desc);
        if (input.peek().id() == token_id::OP_ASSIGN) {
            // optional assignment expression
            input.advance();
            fst_exp_ = new expr;
            parse_use(fst_exp_);
            
            // mov expr result to variable
            context.generate("mov", fst_exp_->place(), "", 
                             desc.address_str());
        } else if (input.peek().id() == token_id::OP_LSBRAC) {
            // optional array declaration
            input.advance();
            dims = new int_list;
            parse_use(dims);
            advance_if(token_id::OP_RSBRAC);
            
            string arr_type = context.table()
                    .register_array(type_, dims->items());
            desc.type(arr_type);
        }
    }
    
    // if there are more
    if (input.peek().id() == token_id::DELIM_COMMA) {
        input.advance();
        list_ = new decl_list(type_);
        parse_use(list_);
        
    }
    advance_if(token_id::DELIM_SEMI);
    
exit:
    if (nullptr != list_) delete list_;
    if (nullptr != fst_exp_) delete fst_exp_;
    if (nullptr != dims) delete dims;
    return res;
}

bool decl_st_part::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::OP_ASSIGN
        || cur_tk.id() == token_id::OP_LSBRAC
        || cur_tk.id() == token_id::DELIM_COMMA
        || cur_tk.id() == token_id::DELIM_SEMI;
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
    
    {
        // function return
        if (!oexp->empty()) {
            // mov return value in eax register
            context.generate("mov", oexp->actual_nt()->place(), "", "eax");
        }
        
        // jump to caller, handled in func_def_part, just jmp to there
        int addr = context.generate("jmp", "", "", "");
        this->nextlist_.push_back(addr);
    }

exit:
    if (nullptr != oexp) delete oexp;
    return res;
}

bool ret_st::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::REV_RETURN;
}

/*
 * statements implementation
 */
bool statements::can_accept(token cur_tk)
{
    statement st;
    return st.can_accept(cur_tk);
}

bool statements::parse(tkstream& input, analyze_context& context)
{
    bool res = true;
    
    statement st;
    statements sts;
    
    if (st.can_accept(input.peek())) {
        if (!st.parse(input, context)) {
            // ignore the error, seek to next ';'
            // try to continue.
            while(input.peek().good()
                    && input.peek().id() != token_id::DELIM_SEMI) {
                input.advance();
            }
            input.advance();
        }
    }
    
    if (sts.can_accept(input.peek())) {
        
        int addr = context.next_address();
        parse_use(&sts);
        
        context.back_patch(st.nextlist(), addr);
        this->nextlist_ = sts.nextlist();
    } else {
        this->nextlist_ = st.nextlist();
    }
    
    exit:
    return res;
}