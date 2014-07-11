#include <sstream>

#include "analyzer/acc_types.h"
#include "analyzer/analyze_context.h"
#include "analyzer/symbol_table.h"

using std::istringstream;
using std::ostringstream;

array_desc::array_desc()
{
    base_ = "";
    total_cnt_ = 0;
}

array_desc::array_desc(const string& array_type)
{
    size_t pos = array_type.find_first_of("[");
    base_ = array_type.substr(0, pos);
    
    int dim;
    total_cnt_ = 1;
    istringstream iss(array_type.substr(pos+1));
    while (iss >> dim) {
        iss.ignore(1, ',');
        dimensions_.push_back(dim);
    }
}

array_desc::array_desc(const string& base_type, const vector<int>& dims)
{
    base_ = base_type;
    dimensions_ = dims;
    total_cnt_ = 1;
    for (auto dim : dims)
    {
        total_cnt_ *= dim;
    }
}

int array_desc::element_size() const
{
    return size_of(base_);
}

int array_desc::elements_count() const
{
    return total_cnt_;
}

std::string array_desc::element_type() const
{
    return base_;
}

const vector< int > array_desc::dimensions() const
{
    return dimensions_;
}

int array_desc::up_bound(int dim) const
{
    return dimensions_[dim];
}

std::string array_desc::type_string() const
{
    return array_type(base_, dimensions_);
}

variable_desc::variable_desc(const string& name, const string& type)
    : name_(name), type_(type), place_(name)
{
    offset_ = 0;
    is_param_ = false;
}

const string variable_desc::address_str() const
{
    ostringstream oss;
    oss << place_ << "+" << offset_;
    return oss.str();
}

function_desc::function_desc()
{
    entry_ = -1;
    parent_ = nullptr;
}

function_desc::function_desc(symbol_table* parent, const string& name, 
              const string& ret_type, const vector<func_arg>& args)
    : name_(name), args_(args), ret_type_(ret_type)
{
    entry_ = -1;
    parent_ = parent;
}

void function_desc::entry(int new_entry)
{
    if (new_entry != -1) {
        // clear waiting list.
        if (nullptr != parent_)
            parent_->context()->back_patch(waiting_, new_entry);
    }
    entry_ = new_entry;
}

string function_desc::signature() const
{
    ostringstream oss;
    oss << name() << "(";
    bool first = true;
    for (func_arg arg : args_)
    {
        if (first) {
            first = false;
        } else {
            oss << ",";
        }
        oss << arg.type;
    }
    oss << ")";
    return oss.str();
}

const_desc::const_desc()
{
    type_ = "";
    value_ = "";
    
    place_ = value_;
}

const_desc::const_desc(token tkconst)
{
    type_ = type_of(tkconst);
    value_ = tkconst.text();
    
    place_ = value_;
}


string signature(const string& name, const vector<string> para_types)
{
    ostringstream oss;
    oss << name << "(";
    bool first = true;
    for (string tp : para_types)
    {
        if (first) {
            first = false;
        } else {
            oss << ",";
        }
        oss << tp;
    }
    oss << ")";
    return oss.str();
}

int precise(const string &type)
{
    if (type == "char") {
        return 0;
    } else if (type == "int") {
        return 1;
    } else if (type.back() == '*') {
        return 1;
    } else if (type == "float") {
        return 2;
    } else {
        return -1;
    }
}

int size_of(const string& type)
{
    if (type == "char") {
        return 1;
    } else if (type == "int") {
        return 4;
    } else if (type == "float") {
        return 8;
    }
    
    // pointers
    if (type.back() == '*') {
        return 4;
    }
    
    // compound types
    size_t star_pos = type.find_first_of('[');
    if (star_pos != string::npos) {
        array_desc ad(type);
        return ad.element_size() * ad.elements_count();
    }
    return 0;
}

string up_convert(const string& type1, const string& type2)
{
    int p1 = precise(type1);
    int p2 = precise(type2);
    
    return (p1 >= p2 ? type1 : type2);
}

string type_of(token tkconst)
{
    switch (tkconst.id())
    {
    case token_id::CONST_INT:
        return "int";
    case token_id::CONST_FLOAT:
        return "float";
    case token_id::CONST_STR:
        return "char*";
    case token_id::CONST_BOOL:
        return "bool";
    default:
        return "unknown";
    }
}

bool is_array(const string& type)
{
    return string::npos != type.find_first_of('[');
}

string array_type(const string& base_type, const vector<int>& dims)
{
    ostringstream type;
    type << base_type << "[";
    for (auto dim : dims)
    {
        type << dim << ",";
    }
    type << "]";
    return type.str();
}