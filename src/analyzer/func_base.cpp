#include "analyzer/analyze_context.h"
#include "analyzer/func_base.h"
#include "analyzer/list_base.h"
#include "analyzer/expression.h"
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

bool func_def_part::can_accept(token cur_tk)
{
    return cur_tk.id() == token_id::OP_LPAREN;
}

bool func_def_part::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    func_entry entry;
    opt_fparams *fps = new opt_fparams;
    block_st *bst = new block_st;
    
    advance_if(token_id::OP_LPAREN);
    parse_use(fps);
    advance_if(token_id::OP_RPAREN);
    
    // declaration
    {
        string sign;
        if (!context.table().new_function(name_, ret_type_, fps->actual_nt()->items(), sign)) {
            entry = context.table().find_func_entry(sign);
            
            context.on_error("functions that differ only in their return type cannot be overloaded",
                             name_tk_.position());
            
            string msg = "previous declaration: \n\t" + entry->second.ret_type() + " " + sign + ";";
            context.on_note(msg);
            res = false;
            goto exit;
        }
        entry = context.table().find_func_entry(sign);
    }
    
    // implementation
    if (input.peek().id() == token_id::DELIM_SEMI) {
        sign_ = true;
        input.advance();
    } else {
        // function body
        sign_ = false;
        // enter function
        context.enter_func(entry);
        int addr = context.next_address();
        //    save registers
        context.generate("push", "ebp", "", "");
        context.generate("mov", "esp", "", "ebp");
        context.generate("pusha", "", "", "");
        //    push a symbol table
        context.table().push_level();
        context.table().new_func_params(entry->second.args());
        //    statements
        parse_use(bst);
        // function return;
        //    pop registers
        context.back_patch(bst->nextlist(), context.next_address());
        context.generate("popa", "", "", "");
        context.generate("mov", "ebp", "", "esp");
        context.generate("pop", "ebp", "", "");
        //    total args size
        int args_size = 0;
        for (auto arg : entry->second.args())
        {
            args_size += size_of(arg.type);
        }
        context.generate("ret", args_size, "", "");
        //    pop symbol table
        context.table().pop_level();
        //    exit function
        context.exit_func();
        
        entry->second.entry(addr);
    }
    
exit:
    if (nullptr != fps) delete fps;
    if (nullptr != bst) delete bst;
    return res;
}