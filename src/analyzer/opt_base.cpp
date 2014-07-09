#include "analyzer/opt_base.h"
#include "lex/tkstream.h"

bool opt_base::parse(tkstream& input, analyze_context& context)
{
    if (!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    non_terminal* nt = nullptr;
    
    nt = create_non_terminal();
    if (nt->can_accept(input.peek())) {
        if (!nt->parse(input, context)) {
            res = false;
            goto exit;
        }
    }
    
exit:
    if(nullptr != nt) delete nt;
    return res;
}

bool opt_base::accept_empty()
{
    return true;
}