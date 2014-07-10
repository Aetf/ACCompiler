#include <cstdio>
#include "analyzer/symbol_table.h"

symbol_table::symbol_table()
{
    // push the global table
    push_level();
}

int symbol_table::push_level()
{
    variable_table table;
    table_stack.push_back(table);
    
    return curr_level();
}

bool symbol_table::pop_level()
{
    if (table_stack.size() > 1) {
        table_stack.pop_back();
        return true;
    }
    return false;
}

int symbol_table::curr_level()
{
    return table_stack.size() - 1;
}

bool symbol_table::new_variable(const string& name, const string& type)
{
    if (curr_level().count(name) != 0) return false;
    
    symbol_item_variable item(name, type);
    curr_table()[name] = item;
    return true;
}

string symbol_table::new_temporary(const string& type)
{
    char c_name[15];
    snprintf(c_name, 15, "T%d", temp_seed_++);
    
    new_variable(string(c_name), type);
    return string(c_name);
}

symbol_entry symbol_table::find_entry(const string& name)
{
    for (auto ptb = table_stack.rbegin(); ptb != table_stack.rend(); ptb++)
    {
        symbol_entry entry = ptb->find(name);
        if (entry != ptb->end())
            return entry;
    }
    return table_stack.front().end();
}

bool symbol_table::valid_entry(symbol_entry entry)
{
    return entry == table_stack.front().end();
}

variable_table& symbol_table::curr_table()
{
    return table_stack.back();
}
