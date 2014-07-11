#include <cstdio>
#include "analyzer/symbol_table.h"
#include "lex/token.h"

symbol_table::symbol_table(analyze_context* pContext)
{
    pContext_ = pContext;
    // push the global table
    push_level();
}

int symbol_table::push_level()
{
    variable_table table;
    stack_.push_back(table);
    
    return curr_level();
}

bool symbol_table::pop_level()
{
    if (stack_.size() > 1) {
        stack_.pop_back();
        return true;
    }
    return false;
}

int symbol_table::curr_level()
{
    return stack_.size() - 1;
}

string symbol_table::new_const(token tkconst)
{
    string type = type_of(tkconst);
    auto ent = consts_.find(type+tkconst.text());
    if (ent != consts_.end()) {
        return ent->second.place();
    }
    
    const_desc desc(tkconst);
    consts_[type+tkconst.text()] = desc;
    return desc.place();
}

bool symbol_table::new_variable(const string& name, const string& type)
{
    symbol_entry ent;
    return new_variable(name, type, ent);
}

bool symbol_table::new_variable(const string& name, const string& type, symbol_entry& entry)
{
    symbol_entry ent = curr_table().find(name);
    if (ent != curr_table().end()
        && !ent->second.is_parameter())
    {
        return false;
    }
    
    variable_desc item(name, type);
    curr_table()[name] = item;
    entry = curr_table().find(name);
    return true;
}

bool symbol_table::new_function(const string& name, const string& ret_type,
                                const vector<func_arg>& args, string& sign)
{
    function_desc item(this, name, ret_type, args);
    sign = item.signature();
    
    func_entry entry = find_func_entry(sign);
    if (valid_entry(entry)) {
        return entry->second.ret_type() == ret_type;
    }
    
    item.entry(-1);
    functions_[item.signature()] = item;
    
    // add parameter as variable
    int offset = 0;
    for (auto arg : args)
    {
        symbol_entry arg_ent;
        new_variable(arg.name, arg.type, arg_ent);
        arg_ent->second.place("ebp");
        arg_ent->second.offset(offset);
        arg_ent->second.is_parameter(true);
        offset += size_of(arg.type);
    }
    return true;
}

bool symbol_table::new_function(const string& name, const string& ret_type, string& sign)
{
    vector<func_arg> args;
    return new_function(name, ret_type, args, sign);
}

string symbol_table::register_array(const string& base_type, const vector<int>& dims)
{
    array_desc ad(base_type, dims);
    arrays_[ad.type_string()] = ad;
    return ad.type_string();
}

string symbol_table::new_temporary(const string& type)
{
    char c_name[15];
    snprintf(c_name, 15, "T%d", temp_seed_++);
    
    symbol_entry ent;
    new_variable(string(c_name), type, ent);
    return string(c_name);
}

symbol_entry symbol_table::find_entry(const string& name)
{
    for (auto ptb = stack_.rbegin(); ptb != stack_.rend(); ptb++)
    {
        symbol_entry entry = ptb->find(name);
        if (entry != ptb->end())
            return entry;
    }
    return stack_.front().end();
}

func_entry symbol_table::find_func_entry(const string& sign)
{
    return functions_.find(sign);
}

bool symbol_table::valid_entry(symbol_entry entry)
{
    return entry != stack_.front().end();
}

bool symbol_table::valid_entry(func_entry entry)
{
    return entry != functions_.end();
}


variable_table& symbol_table::curr_table()
{
    return stack_.back();
}
