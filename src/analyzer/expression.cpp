#include <stack>

#include "analyzer/analyze_context.h"
#include "analyzer/expression.h"
#include "analyzer/list_base.h"

#include "analyzer/helpers.h"

using std::stack;

bool expr::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto lv = new expr_lv0;
    auto h = new expr_h;
    parse_use(lv);
    parse_use(h);
    
    // Generate code
    // only assignment operator here
    {
        if (!h->empty()) {
            // reverse
            stack<expr_h*> st;
            auto ph = h;
            while (nullptr != ph && !ph->empty()) {
                st.push(ph);
                ph = ph->next();
            }
            // process
            while (st.size() > 1) {
                ph = st.top(); st.pop();
                // generate
                context.generate(ph->op_token().text(),
                                 ph->rh_expr()->place(), "",
                                 st.top()->rh_expr()->place());
            }
            // the last one
            ph = st.top(); st.pop();
            context.generate(ph->op_token().text(),
                             ph->rh_expr()->place(), "",
                             lv->place());
        }
        
        this->place_ = lv->place();
        this->type_ = lv->type();
    }

exit:
    if (nullptr != lv) delete lv;
    if (nullptr != h) delete h;
    return res;
}

bool expr::can_accept(token cur_tk)
{
    expr_lv0 lv0;
    return lv0.can_accept(cur_tk);
}

bool expr_lv0::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto lv = new expr_lv1;
    auto h = new expr_lv0_h;
    parse_use(lv);
    parse_use(h);
    
    // Generate code
    {
        auto ph = h;
        string tmp = lv->place();
        string tp = lv->type();
        while (nullptr != ph && !ph->empty()) {
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
    if (nullptr != lv) delete lv;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv0::can_accept(token cur_tk)
{
    expr_lv1 lv;
    return lv.can_accept(cur_tk);
}

bool expr_lv1::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto lv = new expr_lv2;
    auto h = new expr_lv1_h;
    parse_use(lv);
    parse_use(h);
    
    // Generate code
    {
        auto ph = h;
        string tmp = lv->place();
        string tp = lv->type();
        while (nullptr != ph && !ph->empty()) {
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
    if (nullptr != lv) delete lv;
    if (nullptr != h) delete h;
    return res;
}

bool expr_lv1::can_accept(token cur_tk)
{
    expr_lv2 lv;
    return lv.can_accept(cur_tk);
}

bool expr_lv2::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    expr_item *item = nullptr;
    token op_tk;
    string type;
    string place;
    
    switch (input.peek().id())
    {
    case token_id::OP_ADDR:
    case token_id::OP_SUB:
    case token_id::OP_MUL:
        input >> op_tk;
        break;
    default:
        break;
    }
    
    if (input.peek().id() == token_id::OP_LPAREN) {
        // sub expr
        input.advance();
        expr exp;
        parse_use(&exp);
        advance_if(token_id::OP_RPAREN);
        
        place = exp.place();
        type = exp.type();
    }
    else {
        item = new expr_item;
        parse_use(item);
        
        place = item->place();
        type = item->type();
    }
    
    // Generate code
    {
        string tmp;
        string tp = type;
        if (op_tk.good()) {
            tmp = context.table().new_temporary(type);
            switch (op_tk.id())
            {
                case token_id::OP_ADDR:
                    tp += "*";
                    context.generate(op_tk.text(), place, "", tmp);
                    break;
                case token_id::OP_MUL:
                    tp.erase(tp.size() - 1);
                    context.generate("[]", place, 0, tmp);
                    break;
                default:
                    context.generate(op_tk.text(), place, "", tmp);
                    break;
            }
        } else {
            tmp = place;
        }
        this->place_ = tmp;
        this->type_ = tp;
    }
    
exit:
    if (nullptr != item) delete item;
    return res;
}

bool expr_lv2::can_accept(token cur_tk)
{
    expr_item item;
    return item.can_accept(cur_tk)
        || cur_tk.id() == token_id::OP_ADDR
        || cur_tk.id() == token_id::OP_SUB
        || cur_tk.id() == token_id::OP_MUL
        || cur_tk.id() == token_id::OP_LPAREN;
}

bool expr_item::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    opt_aparams *ap = new opt_aparams;
    array_dim *ad = new array_dim;
    
    if (input.peek().iscontant()) {
        // constant
        token tkconst;
        input >> tkconst;
        
        this->place_ = context.table().new_const(tkconst);
        this->type_ = type_of(tkconst);
    }
    else if (input.peek().isidentifier()) {
        // id + opt [] or ()
        token id;
        extract_to(token_id::IDENTIFIER, id);
        
        if (input.peek().id() == token_id::OP_LPAREN) {
            input.advance();
            parse_use(ap);
            advance_if(token_id::OP_RPAREN);
            // function call
            if (!function_call(id , ap, context)) {
                res = false;
                goto exit;
            }
            
        } else if (input.peek().id() == token_id::OP_LSBRAC) {
            input.advance();
            parse_use(ad);
            advance_if(token_id::OP_RSBRAC);
            // array access
            if (!array_access(id, ad, context)) {
                res = false;
                goto exit;
            }
            
        } else {
            // simple variable
            variable_desc desc;
            if (!context.entry_of(id.text(), desc)) {
                context.on_error("Undefined variable " + id.text(), id.position());
                res = false;
                goto exit;
            }
            
            this->place_ = desc.address_str();
            this->type_ = desc.type();
        }
    }
    else {
        context.on_error("Expected expression here.");
        res = false;
        goto exit;
    }
    
exit:
    if (nullptr != ap) delete ap;
    if (nullptr != ad) delete ad;
    return res;
}

bool expr_item::function_call(token id, opt_aparams *ap, analyze_context &context)
{
    bool res = true;
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
    string sig = signature(id.text(), para_types);
    func_entry entry = context.table().find_func_entry(sig);
    if (!context.table().valid_entry(entry)) {
        // not defined yet
        context.on_error("Undefined function " + sig);
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
    
exit:
    return res;
}

bool expr_item::array_access(token id, array_dim* ad, analyze_context& context)
{
    // array element
    variable_desc desc;
    if (!context.entry_of(id.text(), desc)) {
        context.on_error("Undefined variable " + id.text());
        return false;
    }
    // check if match the definition
    if (!is_array(desc.type())) {
        context.on_error("Only array type can use operator [] ");
        return false;
    }
    // check dimension count
    array_desc adesc(desc.type());
    if (adesc.dimensions().size() != ad->items().size()) {
        ostringstream oss;
        oss << "Array dimension mismatch " << std::endl;
        oss << "Need: " << adesc.elements_count() << std::endl;
        oss << "Found: " << ad->items().size();
        context.on_error(oss.str());
        return false;
    }
    // generate code to calc offset
    string offt = context.table().new_temporary("int");
    context.generate("mov", ad->items()[0].place(), "", offt);
    for(size_t i = 1; i!= ad->items().size(); i++)
    {
        context.generate("*", offt, adesc.up_bound(i), offt);
        context.generate("+", offt, ad->items()[i].place(), offt);
    }
    context.generate("*", offt, adesc.element_size(), offt);
    // generate code
    this->type_ = adesc.element_type();
    this->place_ = context.table().new_temporary(adesc.element_type());
    context.generate("[]", desc.address_str(), offt, this->place_);
    
    return true;
}

bool expr_item::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::IDENTIFIER
        || cur_tk.iscontant();
}

expr_h::~expr_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    if (nullptr != nxt_h_) delete nxt_h_;
}

bool expr_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    empty_ = true;
    
    switch (input.peek().id())
    {
        case token_id::OP_ASSIGN:
            input >> op_token_;
            break;
        default:
            break;
    }
    
    if (op_token_.good()) {
        empty_ = false;
        rh_expr_ = new expr_lv0;
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

expr_lv0_h::~expr_lv0_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    if (nullptr != nxt_h_) delete nxt_h_;
}

bool expr_lv0_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    empty_ = true;
    switch (input.peek().id())
    {
        case token_id::OP_ADD:
        case token_id::OP_SUB:
            input >> op_token_;
            break;
        default:
            break;
    }
    
    if (op_token_.good()) {
        empty_ = false;
        rh_expr_ = new expr_lv1;
        parse_use(rh_expr_);
        
        nxt_h_ = new expr_lv0_h;
        parse_use(nxt_h_);
    }
    
    exit:
    return res;
}

bool expr_lv0_h::can_accept(token cur_tk)
{
    return true;
}

expr_lv1_h::~expr_lv1_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    if (nullptr != nxt_h_) delete nxt_h_;
}

bool expr_lv1_h::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    rh_expr_ = nullptr;
    nxt_h_ = nullptr;
    empty_ = true;
    switch (input.peek().id())
    {
        case token_id::OP_MUL:
        case token_id::OP_DIV:
            input >> op_token_;
            break;
        default:
            break;
    }
    
    
    if (op_token_.good()) {
        empty_ = false;
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

bool bool_expr::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto lv = new bool_expr_lv1;
    auto h = new bool_expr_h;
    
    parse_use(lv);
    parse_use(h);
    
    // Generate code
    {
        auto ph = h;
        this->true_list_ = lv->true_list();
        if (!ph->empty()) {
            context.back_patch(lv->false_list(), h->rh_entry());
            
            while (ph->next() != nullptr && !ph->next()->empty()) {
                this->true_list().insert(this->true_list().begin(),
                                         ph->rh_expr()->true_list().begin(),
                                         ph->rh_expr()->true_list().end());
                context.back_patch(ph->rh_expr()->false_list(), ph->next()->rh_entry());
                
                ph = ph->next();
            }
            if (!ph->empty()) {
                this->true_list().insert(this->true_list().begin(),
                                            ph->rh_expr()->true_list().begin(),
                                            ph->rh_expr()->true_list().end());
                this->false_list().insert(this->false_list().begin(),
                                            ph->rh_expr()->false_list().begin(),
                                            ph->rh_expr()->false_list().end());
            }
        } else {
            this->false_list_ = lv->false_list();
        }
    }
    
exit:
    if (nullptr != lv) delete lv;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr::can_accept(token cur_tk)
{
    bool_expr_lv1 lv;
    return lv.can_accept(cur_tk);
}

bool bool_expr_lv1::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto lv = new bool_expr_lv2;
    auto h = new bool_expr_lv1_h;
    
    parse_use(lv);
    parse_use(h);
    
    // Generate code
    {
        bool_expr_lv1_h *ph = h;
        this->false_list_ = lv->false_list();
        if (!ph->empty()) {
            context.back_patch(lv->true_list(), h->rh_entry());
            
            while (ph->next() != nullptr && !ph->next()->empty()) {
                this->false_list().insert(this->false_list().begin(),
                                          ph->rh_expr()->false_list().begin(),
                                          ph->rh_expr()->false_list().end());
                context.back_patch(ph->rh_expr()->true_list(), ph->next()->rh_entry());
                
                ph = ph->next();
            }
            if (!ph->empty()) {
                this->true_list().insert(this->true_list().begin(),
                                        ph->rh_expr()->true_list().begin(),
                                        ph->rh_expr()->true_list().end());
                this->false_list().insert(this->false_list().begin(),
                                        ph->rh_expr()->false_list().begin(),
                                        ph->rh_expr()->false_list().end());
            }
        } else {
            this->true_list_ = lv->true_list();
        }
    }
    
exit:
    if (nullptr != lv) delete lv;
    if (nullptr != h) delete h;
    return res;
}

bool bool_expr_lv1::can_accept(token cur_tk)
{
    bool_expr_lv2 lv;
    return lv.can_accept(cur_tk);
}

bool bool_expr_lv2::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    
    auto item = new bool_expr_item;
    token op_tk;
    
    switch (input.peek().id())
    {
        case token_id::OP_NOT:
            input >> op_tk;
            break;
        default:
            break;
    }
    parse_use(item);
    
    // Generate code
    {
        if (op_tk.id() == token_id::OP_NOT) {
            this->true_list_ = item->false_list();
            this->false_list_ = item->true_list();
        } else {
            this->true_list_ = item->true_list();
            this->false_list_ = item->false_list();
        }
    }
    
exit:
    if (nullptr != item) delete item;
    return res;
}

bool bool_expr_lv2::can_accept(token cur_tk)
{
    bool_expr_item item;
    return cur_tk.id() == token_id::OP_NOT
    || item.can_accept(cur_tk);
}

bool bool_expr_item::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    auto exp = new expr_lv2;
    
    if (input.peek().id() == token_id::CONST_BOOL) {
        // constant
        token c;
        input >> c;
        this->true_list().push_back(context.next_address());
        this->false_list().push_back(context.next_address()+1);
        context.generate("jnz", c.text(), "", "");
        context.generate("jmp", "", "", "");
    }
    else if (input.peek().id() == token_id::OP_LPAREN) {
        // sub bool_expr
        input.advance();
        bool_expr bexp;
        parse_use(&bexp);
        advance_if(token_id::OP_RPAREN);
        
        this->true_list_ = bexp.true_list();
        this->false_list_ = bexp.false_list();
    }
    else if (exp->can_accept(input.peek())) {
        // expr op expr
        parse_use(exp);
        
        token op;
        switch (input.peek().id())
        {
            case token_id::OP_EQ:
            case token_id::OP_NE:
            case token_id::OP_GE:
            case token_id::OP_LE:
            case token_id::OP_GT:
            case token_id::OP_LT:
                input >> op;
                break;
            default:
                break;
        }
        
        if (op.good()) {
            expr_lv2 rh;
            parse_use(&rh);
            
            this->true_list().push_back(context.next_address());
            this->false_list().push_back(context.next_address()+1);
            context.generate("j" + op.text(),
                             exp->place(), rh.place(),
                             "");
            context.generate("jmp", "", "", "");
        } else {
            this->true_list().push_back(context.next_address());
            this->false_list().push_back(context.next_address()+1);
            context.generate("jnz", exp->place(), "", "");
            context.generate("jmp", "", "", "");
        }
    }
    
exit:
    if (nullptr != exp) delete exp;
    return res;
}

bool bool_expr_item::can_accept(token cur_tk)
{
    expr exp;
    return exp.can_accept(cur_tk)
        || cur_tk.id() == token_id::CONST_BOOL
        || cur_tk.id() == token_id::OP_LPAREN;
}

bool_expr_h::~bool_expr_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    if (nullptr != nxt_h_) delete nxt_h_;
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
    empty_ = true;
    if (input.peek().id() == token_id::OP_OR) {
        input >> op_token_;
    }
    
    if (op_token_.good()) {
        empty_ = false;
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

bool_expr_lv1_h::~bool_expr_lv1_h()
{
    if (nullptr != rh_expr_) delete rh_expr_;
    if (nullptr != nxt_h_) delete nxt_h_;
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
    empty_ = true;
    if (input.peek().id() == token_id::OP_AND) {
        input >> op_token_;
    }
    
    if (op_token_.good()) {
        empty_ = false;
        rh_expr_ = new bool_expr_lv2;
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