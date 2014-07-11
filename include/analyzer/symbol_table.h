#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <deque>
#include <sstream>
#include <string>
#include <unordered_map>

#include "analyzer/acc_types.h"
#include "analyzer/func_base.h"

using std::deque;
using std::ostringstream;
using std::string;
using std::unordered_map;


using variable_table = unordered_map<string, variable_desc>;
using function_table = unordered_map<string, function_desc>;
using array_table = unordered_map<string, array_desc>;
using const_table = unordered_map<string, const_desc>;
using temporary_table = unordered_map<int, variable_desc>;
using table_stack = deque<variable_table>;
using symbol_entry = variable_table::iterator;
using func_entry = function_table::iterator;

class symbol_table
{
public:
    symbol_table(analyze_context* pContext);
    
    int push_level();
    bool pop_level();
    int curr_level();
    
    string new_const(token tkconst);
    bool new_variable(const string& name, const string& type, symbol_entry& entry);
    bool new_variable(const string& name, const string& type);
    
    bool new_function(const string& name, const string& ret_type,
                      const vector<func_arg>& args, string& sign);
    
    bool new_function(const string& name, const string& ret_type, string& sign);
    
    string new_temporary(const string& type);
    
    string register_array(const string& base_type, const vector<int>& dims);
    
    symbol_entry find_entry(const string& name);
    bool valid_entry(symbol_entry entry);
    
    func_entry find_func_entry(const string& sign);
    bool valid_entry(func_entry entry);
    
    analyze_context* context() { return pContext_; }
protected:
    variable_table& curr_table();
    
private:
    
    int temp_seed_;
    table_stack stack_;
    function_table functions_;
    array_table arrays_;
    const_table consts_;
    
    analyze_context* pContext_;
};

#endif //SYMBOL_TABLE_H