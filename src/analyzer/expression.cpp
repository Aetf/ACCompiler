#include "analyzer/analyze_context.h"
#include "analyzer/expression.h"
#include "analyzer/list_base.h"

#include "analyzer/helpers.h"

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
    
    // Generate code
    {
        expr_h *ph = h;
        string tmp = lv1->place();
        string tp = lv1->type();
        while (nullptr != ph) {
            // determin type
            tp = up_convert(tp, ph->rh_expr()->type());
            // TODO: convert code
            // generate
            string tmp2 = context.table().new_temporary(tp);
            context.generate(ph->op_token().text(),
                             tmp, ph->rh_expr()->place(),
                             tmp2);
            tmp = tmp2;
            
            ph = ph->next();
        }
        this->place_ = tmp;
        this->type_ = tp;
    }
    
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

expr_h::~expr_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    while (nullptr != nxt_h_) {
        expr_h *t = nxt_h_;
        nxt_h_ = nxt_h_->next();
        delete t;
    }
}

bool expr_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    
    if (input.peek().id() == token_id::OP_ADD) {
        input >> op_token_;
        
        rh_expr_ = new expr_lv1;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_h;
        parse_use(nxt_h_);
        
    } else if (input.peek().id() == token_id::OP_SUB) {
        input >> op_token_;
        
        rh_expr_ = new expr_lv1;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_h;
        parse_use(nxt_h_);
    }
    
    exit:
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
    
    // Generate code
    {
        expr_lv1_h *ph = h;
        string tmp = lv2->place();
        string tp = lv2->type();
        while (nullptr != ph) {
            // determin type
            tp = up_convert(tp, ph->rh_expr()->type());
            // TODO: convert code
            // generate
            string tmp2 = context.table().new_temporary(tp);
            context.generate(ph->op_token().text(),
                             tmp, ph->rh_expr()->place(),
                             tmp2);
            tmp = tmp2;
            
            ph = ph->next();
        }
        this->place_ = tmp;
        this->type_ = tp;
    }
    
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

expr_lv1_h::~expr_lv1_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    while (nullptr != nxt_h_) {
        expr_lv1_h *t = nxt_h_;
        nxt_h_ = nxt_h_->next();
        delete t;
    }
}

bool expr_lv1_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    
    if (input.peek().id() == token_id::OP_MUL) {
        input >> op_token_;
        
        rh_expr_ = new expr_lv2;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_lv1_h;
        parse_use(nxt_h_);
    } else if (input.peek().id() == token_id::OP_DIV) {
        input >> op_token_;
        
        rh_expr_ = new expr_lv2;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_lv1_h;
        parse_use(nxt_h_);
    }
    
    exit:
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
    
    // Generate code
    {
        expr_lv2_h *ph = h;
        string tmp = lv3->place();
        string tp = lv3->type();
        while (nullptr != ph) {
            // determin type
            tp = up_convert(tp, ph->rh_expr()->type());
            // TODO: convert code
            // generate
            string tmp2 = context.table().new_temporary(tp);
            context.generate(ph->op_token().text(),
                             tmp, ph->rh_expr()->place(),
                             tmp2);
            tmp = tmp2;
            
            ph = ph->next();
        }
        this->place_ = tmp;
        this->type_ = tp;
    }
    
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

expr_lv2_h::~expr_lv2_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    while (nullptr != nxt_h_) {
        expr_lv2_h *t = nxt_h_;
        nxt_h_ = nxt_h_->next();
        delete t;
    }
}

bool expr_lv2_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    
    if (input.peek().id() == token_id::OP_ADDR) {
        input >> op_token_;
    } else if (input.peek().id() == token_id::OP_SUB) {
        input >> op_token_;
    } else if (input.peek().id() == token_id::REV_NOT) {
        input >> op_token_;
    } else if (input.peek().id() == token_id::OP_MUL) {
        input >> op_token_;
    }
    
    if (op_token_.good()) {
        rh_expr_ = new expr_lv3;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_lv2_h;
        parse_use(nxt_h_);
    }
    
    exit:
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
        
        this->place_ = exp->place();
        this->type_ = exp->type();
    } else if (item->can_accept(input.peek())) {
        if (!item->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        this->place_ = item->place();
        this->type_ = item->type();
    } else if (input.peek().iscontant()) {
        token tkconst;
        input >> tkconst;
        this->place_ = context.table().new_const(tkconst);
        this->type_ = type_of(tkconst);
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    
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
        advance_if(token_id::OP_RBRAC);
        
        // function call
        vector<string> para_types;
        //    push parameters
        if (!ap->empty()) {
            for (auto expr : ap->actual_nt()->items())
            {
                context.generate("push", expr.place(), "", "");
                para_types.push_back(expr.type());
            }
        }
        //    call
        func_entry entry = context.table().find_func_entry(signature(id.text(), para_types));
        if (!context.table().valid_entry(entry)) {
            // not defined yet
            context.on_error("Undefined function " + id.text());
            res = false;
            goto exit;
        }        
        if (!entry->second.defined()) {
            // no body yet
            // append to waiting list
            entry->second.wait_for_define(context.next_address());
            context.generate("call", "", "", "");
        } else {
            context.generate("call", "", "", entry->second.entry());
        }
        //    get return value
        this->type_ = entry->second.ret_type();
        this->place_ = context.table().new_temporary(entry->second.ret_type());
        context.generate("mov", "eax", "", this->place_);
        
    } else if (input.peek().id() == token_id::OP_LSBRAC) {
        parse_use(ad);
        advance_if(token_id::OP_RSBRAC);
        
        // array element
        variable_desc desc;
        if (!context.entry_of(id.text(), desc)) {
            context.on_error("Undefined variable " + id.text());
            res = false;
            goto exit;
        }
        // check if match the definition
        if (!is_array(desc.type())) {
            context.on_error("Only array type can use operator [] ");
            res = false;
            goto exit;
        }
        array_desc adesc(desc.type());
        if (adesc.elements_count() != ad->items().size()) {
            ostringstream oss;
            oss << "Array dimension mismatch " << std::endl;
            oss << "Need: " << adesc.elements_count() << std::endl;
            oss << "Found: " << ad->items().size();
            context.on_error(oss.str());
            res = false;
            goto exit;
        }
        // generate code to calc offset
        string offt = context.table().new_temporary("int");
        context.generate("mov", ad->items()[0].place(), "", offt);
        for(size_t i = 1; i!= ad->items().size(); i++)
        {
            context.generate("*", offt, adesc.dimensions()[0], offt);
            context.generate("+", offt, ad->items()[i].place(), offt);
        }
        this->type_ = adesc.element_type();
        this->place_ = context.table().new_temporary(adesc.element_type());
        context.generate("[]", desc.address_str(), offt, this->place_);
    } else {
        // simple variable
        variable_desc desc;
        if (!context.entry_of(id.text(), desc)) {
            context.on_error("Undefined variable " + id.text());
            res = false;
            goto exit;
        }
        
        this->place_ = desc.address_str();
        this->type_ = desc.type();
    }
    
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
    
    // Generate code
    {
        bool_expr_h *ph = h;
        this->true_list() = lv1->true_list();
        context.back_patch(lv1->false_list(), h->rh_entry());
        while (nullptr != ph && nullptr != ph->next()) {
            this->true_list().insert(this->true_list().begin(),
                                     ph->rh_expr()->true_list().begin(),
                                     ph->rh_expr()->true_list().end());
            context.back_patch(ph->rh_expr()->false_list(), ph->next()->rh_entry());
            
            ph = ph->next();
        }
        this->true_list().insert(this->true_list().begin(),
                                 ph->rh_expr()->true_list().begin(),
                                 ph->rh_expr()->true_list().end());
        this->false_list().insert(this->false_list().begin(),
                                  ph->rh_expr()->false_list().begin(),
                                  ph->rh_expr()->false_list().end());
    }
    
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

bool_expr_h::~bool_expr_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    while (nullptr != nxt_h_) {
        bool_expr_h *t = nxt_h_;
        nxt_h_ = nxt_h_->next();
        delete t;
    }
}

bool bool_expr_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    rh_entry_ = -1;
    
    if (input.peek().id() == token_id::REV_OR) {
        input >> op_token_;
    }
    
    if (op_token_.good()) {
        rh_expr_ = new bool_expr_lv1;
        rh_entry_ = context.next_address();
        parse_use(rh_expr_);
        
        nxt_h_ = new bool_expr_h;
        parse_use(nxt_h_);
    }
    
    exit:
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
    
    bool_expr_lv3 *lv3 = new bool_expr_lv3;
    bool_expr_lv1_h *h = new bool_expr_lv1_h;
    parse_use(lv3);
    parse_use(h);
    
    // Generate code
    {
        bool_expr_lv1_h *ph = h;
        this->true_list() = lv3->true_list();
        context.back_patch(lv3->false_list(), h->rh_entry());
        while (nullptr != ph && nullptr != ph->next()) {
            this->true_list().insert(this->true_list().begin(),
                                     ph->rh_expr()->true_list().begin(),
                                     ph->rh_expr()->true_list().end());
            context.back_patch(ph->rh_expr()->false_list(), ph->next()->rh_entry());
            
            ph = ph->next();
        }
        this->true_list().insert(this->true_list().begin(),
                                 ph->rh_expr()->true_list().begin(),
                                 ph->rh_expr()->true_list().end());
        this->false_list().insert(this->false_list().begin(),
                                  ph->rh_expr()->false_list().begin(),
                                  ph->rh_expr()->false_list().end());
    }
    
exit:
    if (nullptr != lv3) delete lv3;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv1::can_accept(token cur_tk)
{
    bool_expr_lv3 lv3;
    return lv3.can_accept(cur_tk);
}

bool_expr_lv1_h::~bool_expr_lv1_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    while (nullptr != nxt_h_) {
        bool_expr_lv1_h *t = nxt_h_;
        nxt_h_ = nxt_h_->next();
        delete t;
    }
}

bool bool_expr_lv1_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    rh_entry_ = -1;
    
    if (input.peek().id() == token_id::REV_AND) {
        input >> op_token_;
    }
    
    if (op_token_.good()) {
        rh_expr_ = new bool_expr_lv3;
        rh_entry_ = context.next_address();
        parse_use(rh_expr_);
        
        nxt_h_ = new bool_expr_lv1_h;
        parse_use(nxt_h_);
    }
    exit:
    return res;
}

bool bool_expr_lv1_h::can_accept(token cur_tk)
{
    return true;
}

bool bool_expr_lv3::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    bool_expr_lv2 *exp = new bool_expr_lv2;
    bool_expr *boolexp = new bool_expr;
    if (input.peek().id() == token_id::OP_LBRAC) {
        input.advance();
        parse_use(boolexp);
        advance_if(token_id::OP_RBRAC);
        
        this->true_list().insert(this->true_list().begin(),
                                 boolexp->true_list().begin(), boolexp->true_list().end());
        this->false_list().insert(this->false_list().begin(),
                                  boolexp->false_list().begin(), boolexp->false_list().end());
    } else if (exp->can_accept(input.peek())) {
        if (!exp->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        this->true_list().insert(this->true_list().begin(),
                                 exp->true_list().begin(), exp->true_list().end());
        this->false_list().insert(this->false_list().begin(),
                                  exp->false_list().begin(), exp->false_list().end());
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
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


bool bool_expr_lv2::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    token op;
    
    expr *exp = new expr;
    expr *rh = new expr;
    
    parse_use(exp);
    
    if (input.peek().id() == token_id::OP_EQ) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_NE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_GE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_LE) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_LT) {
        input >> op;
    } else if (input.peek().id() == token_id::OP_GT) {
        input >> op;
    }
    
    if (op.good()) {
        parse_use(rh);
        
        this->true_list().push_back(context.next_address());
        this->false_list().push_back(context.next_address()+1);
        context.generate("j" + op.text(),
                         exp->place(), rh->place(),
                         "");
        context.generate("jmp", "", "", "");
    } else {
        this->true_list().push_back(context.next_address());
        this->false_list().push_back(context.next_address()+1);
        context.generate("jnz", exp->place(), "", "");
        context.generate("jmp", "", "", "");
    }
    
exit:
    if (nullptr != exp) delete exp;
    if (nullptr != rh) delete rh;
    return res;
}

bool bool_expr_lv2::can_accept(token cur_tk)
{
    expr exp;
    return exp.can_accept(cur_tk);
}