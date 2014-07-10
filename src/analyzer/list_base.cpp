#include "analyzer/analyze_context.h"
#include "analyzer/list_base.h"
#include "lex/tkstream.h"

#include "analyzer/helpers.h"

/*
 * list_base implementation
 */
template <typename Item_Type>
bool list_base<Item_Type>::parse(tkstream& input, analyze_context& context)
{
    if(!non_terminal::parse(input, context)) {
        return false;
    }
    
    if(!parse_single(input, context)) {
        return false;
    }
    
    bool res = true;
    list_more_base<Item_Type>* list_more = nullptr;
    
    list_more = create_list_more();
    parse_use(list_more);
    
    // merge the item list
    items().reserve(items().size() + list_more->items().size());
    items().insert(items().end(), list_more->items().begin(), list_more->items().end());

exit:
    if(nullptr != list_more) delete list_more;
    return true;
}


/*
 * list_base_more implementation
 */
template <typename Item_Type>
bool list_more_base<Item_Type>::parse(tkstream& input, analyze_context& context)
{
    if(!non_terminal::parse(input, context)) {
        return false;
    }
    
    bool res = true;
    non_terminal *sep = nullptr;
    list_base<Item_Type> *list = nullptr;
    
    sep = create_sep();
    if(sep->can_accept(input.peek()))
    {   // list_more -> sep list_base
        if(!sep->parse(input, context)) {
            res = false;
            goto exit;
        }
        
        list = create_list();
        parse_use(list);
        
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

template <typename Item_Type>
bool list_more_base<Item_Type>::accept_empty()
{
    return true;
}

template <typename Item_Type>
bool list_more_base<Item_Type>::can_accept(token cur_tk)
{
    return true;
}


/*
 * fparams implementation
 */
list_more_base<func_arg>* fparams::create_list_more()
{
    return new fparams_more;
}

bool fparams::can_accept(token cur_tk)
{
    type_name tp;
    return tp.can_accept(cur_tk);
}

bool fparams::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    token idtk;
    type_name *tp = new type_name;
    
    parse_use(tp);
    
    extract_to(token_id::IDENTIFIER, idtk);
    
    {
        func_arg arg(idtk.text(), tp->type_string());
        items().push_back(arg);
    }
    
exit:
    if (nullptr != tp) delete tp;
    return res;
}


/*
 * fparams_more implementation
 */
non_terminal* fparams_more::create_sep()
{
    return new single_token(token_id::DELIM_COMMA);
}

list_base< func_arg >* fparams_more::create_list()
{
    return new fparams;
}


/*
 * aparams implementation
 */
bool aparams::can_accept(token cur_tk)
{
    expr exp;
    return exp.can_accept(cur_tk);
}

bool aparams::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    expr exp;
    parse_use(&exp);
    
    items().push_back(exp);
exit:
    return res;
}

list_more_base< expr >* aparams::create_list_more()
{
    return new aparams_more;
}


/*
 * aparams_more implementation
 */
non_terminal* aparams_more::create_sep()
{
    return new single_token(token_id::DELIM_COMMA);
}

list_base< expr >* aparams_more::create_list()
{
    return new aparams;
}


/*
 * statements implementation
 */
bool statements::can_accept(token cur_tk)
{
    statement st;
    return st.can_accept(cur_tk);
}

bool statements::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    
    statement st;
    parse_use(&st);
    
    // statement semantic
exit:
    return res;
}

list_more_base< statement >* statements::create_list_more()
{
    return new statements_more;
}


/*
 * statements_more implementation
 */
non_terminal* statements_more::create_sep()
{
    return new epsilon;
}

list_base< statement >* statements_more::create_list()
{
    return new statements;
}


/*
 * decl_list implementation
 */
list_more_base<decl_item>* decl_list::create_list_more()
{
    return new decl_list_more;
}

bool decl_list::can_accept(token cur_tk)
{
    decl_item item;
    return item.can_accept(cur_tk);
}

bool decl_list::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    
    decl_item item;
    parse_use(&item);
    
    items().push_back(item);
exit:
    return res;
}


/*
 * decl_list_more implementation
 */
non_terminal* decl_list_more::create_sep()
{
    return new single_token(token_id::DELIM_COMMA);
}

list_base<decl_item>* decl_list_more::create_list()
{
    return new decl_list;
}


/*
 * array_dim implementation
 */

bool array_dim::can_accept(token cur_tk)
{
    expr exp;
    return exp.can_accept(cur_tk);
}

list_more_base<expr>* array_dim::create_list_more()
{
    return new array_dim_more;
}

bool array_dim::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    expr exp;
    parse_use(&exp);
    
    items().push_back(exp);
exit:
    return true;
}

/*
 * array_dim_more implementation
 */
non_terminal* array_dim_more::create_sep()
{
    return new single_token(token_id::DELIM_COMMA);
}

list_base< expr >* array_dim_more::create_list()
{
    return new array_dim;
}


/*
 * decl_sts implementation
 */
list_more_base<decl_st>* decl_sts::create_list_more()
{
    return new decl_sts_more;
}

bool decl_sts::can_accept(token cur_tk)
{
    decl_st st;
    return st.can_accept(cur_tk);
}

bool decl_sts::parse_single(tkstream& input, analyze_context& context)
{
    bool res = true;
    decl_st st;
    parse_use(&st);
    
    // TODO: decl semantic
    items().push_back(st);
exit:
    return true;
}


/*
 * decl_sts_more implementation
 */
non_terminal* decl_sts_more::create_sep()
{
    return new epsilon;
}

list_base< decl_st >* decl_sts_more::create_list()
{
    return new decl_sts;
}


