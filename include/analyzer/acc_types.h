#ifndef ACC_TYPES_H
#define ACC_TYPES_H

#include <string>
#include <vector>

#include "analyzer/func_base.h"
#include "lex/token.h"

using std::string;
using std::vector;

/**
 * extra array description
 */
class array_desc
{
    vector<int> dimensions_;
    string base_;
    int total_cnt_;
public:
    array_desc();
    array_desc(const string& array_type);
    array_desc(const string& base_type, const vector<int>& dims);
    
    const vector<int> dimensions() const;
    int up_bound(int dim) const;
    string element_type() const;
    int element_size() const;
    int elements_count() const;
    string type_string() const;
};

/**
 * variable description
 */
class variable_desc
{
    string name_;
    string type_;
    string place_;
    int offset_;
    bool is_param_;
public:
    variable_desc(const string& name = "", const string& type = "");
    const string& name() const { return name_; }
    void name(const string& new_name) { name_ = new_name; }
    
    const string& type() const { return type_; }
    void type(const string& new_type) { type_ = new_type; }
    
    const int offset() const { return offset_; }
    void offset(int offset) { offset_ = offset; }
    
    const string place() const { return place_; }
    void place(string place) { place_ = place; }
    
    bool is_parameter() const { return is_param_; }
    void is_parameter(bool value) { is_param_ = value; }
    
    const string address_str() const;
};

/**
 * function description
 */
class symbol_table;
struct function_desc
{
    function_desc();
    function_desc(symbol_table* parent, const string& name,
                  const string& ret_type, const vector<func_arg>& args);
    
    const string& name() const { return name_; }
    void name(const string& new_name) { name_ = new_name; }
    
    const string& ret_type() const { return ret_type_; }
    void ret_type(const string& new_type) { ret_type_ = new_type; }
    
    vector<func_arg> args() {return args_; }
    
    int entry() const { return entry_; }
    void entry(int new_entry);
    
    bool defined() const { return entry_ != -1; }
    
    void wait_for_define(int addr) { waiting_.push_back(addr); }
    
    string signature() const;
    
private:
    string name_;
    vector<func_arg> args_;
    string ret_type_;
    int entry_;
    symbol_table* parent_;
    vector<int> waiting_;
};

/**
 * constant description
 */
class const_desc
{
    string value_;
    string type_;
    string place_;
public:
    const_desc();
    const_desc(token tkconst);
    const string& value() const { return value_; }
    void value(const string& new_value) { value_ = new_value; }
    
    const string& type() const { return type_; }
    void type(const string& new_type) { type_ = new_type; }
    
    const string place() const { return place_; }
    void place(string place) { place_ = place; }
};

/**
 * get the precise level of the given type.
 * used in type auto casting system
 */
int precise(const string &type);

/**
 * get the bytes of a type
 */
int size_of(const string& type);

/**
 * get the type of a literal value
 */
string type_of(token tkconst);

/**
 * if the given type string is an array
 */
bool is_array(const string &type);

/**
 * get a type which is the most precise
 */
string up_convert(const string& type1, const string& type2);

/**
 * calculate a function's signature
 */
string signature(const string& name, const vector<string> para_types);

/**
 * calculate an array's type string
 */
string array_type(const string& base_type, const vector<int>& dims);

#endif // ACC_TYPES_H