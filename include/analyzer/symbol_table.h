#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <deque>
#include <string>
#include <unordered_map>

using std::deque;
using std::string;
using std::unordered_map;


struct symbol_item_variable
{
    symbol_item_variable(const string& name = "", const string& type = "")
    : name_(name), type_(type) { }
    const string& name() const { return name_; }
    void name(const string& new_name) { name_ = new_name; }
    
    const string& type() const { return type_; }
    void type(const string& new_type) { type_ = new_type; }
    
    const string address_str() const { return name_; }
    
private:
    string name_;
    string type_;
};


using variable_table = unordered_map<string, symbol_item_variable>;
using temporary_table = unordered_map<int, symbol_item_variable>;
using table_stack = deque<variable_table>;
using symbol_entry = variable_table::iterator;

class symbol_table
{
public:
    symbol_table();
    
    void push_level();
    bool pop_level();
    int curr_level();
    
    void new_variable(const string& name, const string& type);
    string new_temporary(const string& type);
    
    symbol_entry find_entry(const string& name);
    bool valid_entry(symbol_entry entry);
protected:
    variable_table& curr_table();
    
private:
    
    int temp_seed_;
    table_stack stack_;
};

#endif //SYMBOL_TABLE_H