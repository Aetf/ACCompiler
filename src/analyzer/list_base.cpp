#include "analyzer/analyze_context.h"
#include "analyzer/list_base.h"
#include "lex/tkstream.h"

bool list_base::parse(tkstream& input, analyze_context& context)
{
    if(!non_terminal::parse(input, context)) {
        return false;
    }
    
    if(!parse_single(input, context)) {
        return false;
    }
    
    bool res = true;
    list_more_base* list_more = nullptr;
    
    list_more = create_list_more();
    if(!list_more->can_accept()) {
        context.on_error();
        res = false;
        goto exit;
    }
    
    if (!list_more->parse(input, context)) {
        res = false;
        goto exit;
    }
    
    // merge the item list
    items().reserve(items().size() + list_more->items().size());
    items().insert(items().end(), list_more->items().begin(), list_more->items().end());
    
    if(nullptr != list_more) delete list_more;
    return true;
}

bool list_base::accept_empty()
{
    return false;
}

vector< non_terminal >& list_base::items() const
{
    return items_;
}

bool list_more_base::parse(tkstream& input, analyze_context& context)
{
    if(!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    non_terminal *sep = nullptr;
    list_base *list = nullptr;
    
    sep = create_sep();
    if(sep->can_accept(input.peek()))
    {   // list_more -> sep list_base
        if(!sep->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        list = create_list();
        if(!list->can_accept(input.peek())) {
            context.on_error();
            res = false;
            goto exit;
        }
        if(!list->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        // merge the item list
        items().reserve(items().size() + list->items().size());
        items().insert(items().end(), list->items().begin(), list->items().end());
    }
    else
    {   // list_more_base -> <empty>
        // do nothing
    }
    
exit:
    if(nullptr != sep) delete sep;
    if(nullptr != list) delete list;
    return res;
}

bool list_more_base::accept_empty()
{
    return true;
}

vector<non_terminal>& list_more_base::items() const
{
    return items_;
}

