#include "analyzer/analyze_context.h"
#include "analyzer/func_base.h"
#include "analyzer/list_base.h"
#include "lex/tkstream.h"

#include "analyzer/helpers.h"

func_base::func_base()
{
    entry_ = -1;
}

void func_base::name(const string& new_name)
{
    name_ = new_name;
}

std::string func_base::name() const
{
    return name_;
}

void func_base::entry(int new_entry)
{
    entry_ = new_entry;
}

int func_base::entry() const
{
    return entry_;
}

vector< func_arg >& func_base::args()
{
    return args_;
}

bool func_def::can_accept(token cur_tk)
{
    func_sign sign;
    return sign.can_accept(cur_tk);
}

bool func_def::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    func_sign *sign = new func_sign;
    block_st *st = new block_st;
    
    sign_ = true;
    parse_use(sign);
    
    if (input.peek().id() == token_id::DELIM_SEMI) {
        input.advance();
    } else if (st->can_accept(input.peek())) {
        if (!st->parse(input, context)) {
            res = false;
            goto exit;
        }
    } else {
        context.on_error(input.peek());
        res = false;
        goto exit;
    }
    
    // TODO: Add to symbol table
exit:
    if (nullptr != sign) delete sign;
    if (nullptr != st) delete st;
    return res;
}

bool func_def_part::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::OP_LBRAC;
}

bool func_def_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    type_name *tp = new type_name;
    opt_fparams *fps = new opt_fparams;
    block_st *bst = new block_st;
    
    token idtk;
    parse_use(tp);
    
    extract_to(token_id::IDENTIFIER, idtk);
    
    advance_if(token_id::OP_LBRAC);
    parse_use(fps);
    advance_if(token_id::OP_RBRAC);
    
    // TODO: add to arg list
    // TODO: name
    
    
    if (input.peek().id() == token_id::DELIM_SEMI) {
        sign_ = true;
        input.advance();
    } else {
        sign_ = false;
        parse_use(bst);
        
        // TODO: entry
    }
    
    // TODO: type unknown, defered to starter_more
    
exit:
    if (nullptr != tp) delete tp;
    if (nullptr != fps) delete fps;
    if (nullptr != bst) delete bst;
    return res;
}


bool func_sign::can_accept(token cur_tk)
{
    type_name tp;
    return tp.can_accept(cur_tk);
}

bool func_sign::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    type_name *tp = new type_name;
    opt_fparams *fps = new opt_base< fparams >;
    
    token idtk;
    parse_use(tp);
    
    extract_to(token_id::IDENTIFIER, idtk);
    
    advance_if(token_id::OP_LBRAC);
    parse_use(fps);
    advance_if(token_id::OP_RBRAC);
    
    // TODO: semantic
exit:
    if (nullptr != tp) delete tp;
    if (nullptr != fps) delete fps;
    return res;
}