#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "analyzer/acc_types.h"
#include "analyzer/analyze_context.h"
#include "interm/quadruple.h"
#include "lex/token.h"

namespace bf = boost::filesystem;

using namespace std;
using bf::path;

std::ostream& operator<<(std::ostream& os, const token& tk)
{
    os << "("
    << "id:" << tk.id() << ", "
    << "pos:" << tk.position().line << ":" << tk.position().col << ", ";
    
    if(tk.isreserved())
        os << "-";
    else
        os << tk.text();
    
    os << ")";
    return os;
}

analyze_context::analyze_context(const std::string& output_file)
    :sym_table_(this)
{
    o_path_ = output_file;
    in_func_ = false;
}

void analyze_context::on_error(token cur_tk)
{
    std::cout << "Error: "<< cur_tk << std::endl;
}

void analyze_context::on_error(const string &msg)
{
    std::cout << msg << std::endl;
}

void analyze_context::on_error(const string& msg, const text_pointer& pointer)
{
    ostringstream os;
    string line_;
    os << pointer.line << ":" << pointer.col << ": error: " << msg << '\n'
    << line_ << '\n'
    << string(pointer.col-1, ' ') << '^';
    
    std::cout << os.str() << std::endl;
}

void analyze_context::on_critical(const string &msg)
{
    std::cout << msg << std::endl;
}

void analyze_context::on_critical(const string& msg, const text_pointer& pointer, const string& line)
{
    ostringstream os;
    string line_(line);
    os << pointer.line << ":" << pointer.col << ": error: " << msg << '\n'
    << line_ << '\n'
    << string(pointer.col-1, ' ') << '^';
    
    std::cout << os.str() << std::endl;
}

func_entry analyze_context::curr_func() const
{
    return curr_func_;
}

void analyze_context::enter_func(func_entry entry)
{
    curr_func_ = entry;
    in_func_ = true;
}

void analyze_context::exit_func()
{
    in_func_ = false;
}

int analyze_context::next_address() const
{
    return codes_.size();
}

int analyze_context::generate(const string &op, const string &arg1,
                              const string &arg2, const string &dest)
{
    codes_.push_back(quadruple(codes_.size(), op, arg1, arg2, dest));
    return codes_.size() - 1;
}

int analyze_context::generate(const string &op, int arg1,
             const string &arg2,const string &dest)
{
    ostringstream oss;
    oss << arg1;
    return generate(op, oss.str(), arg2, dest);
}

int analyze_context::generate(const string &op, const string &arg1,
             int arg2, const string &dest)
{
    ostringstream oss;
    oss << arg2;
    return generate(op, arg1, oss.str(), dest);
}

int analyze_context::generate(const string &op, const string &arg1,
             const string &arg2, int dest)
{
    ostringstream oss;
    oss << dest;
    return generate(op, arg1, arg2, oss.str());
}

int analyze_context::generate(const string &op, const string &arg1,
             int arg2, int dest)
{
    ostringstream ossd, oss2;
    ossd << dest; oss2 << arg2;
    return generate(op, arg1, oss2.str(), ossd.str());
}

int analyze_context::generate(const string &op, int arg1,
             const string &arg2, int dest)
{
    ostringstream ossd, oss1;
    ossd << dest; oss1 << arg1;
    return generate(op, oss1.str(), arg2, ossd.str());
}

int analyze_context::generate(const string &op, int arg1,
             int arg2, int dest)
{
    ostringstream oss1, oss2, ossd;
    oss1 << arg1; oss2 << arg2; ossd << dest;
    return generate(op, oss1.str(), oss2.str(), ossd.str());
}

void analyze_context::back_patch(vector<int>& addr_list, int value)
{
    ostringstream oss;
    oss << value;
    for (int ad : addr_list)
    {
        codes_[ad].dest(oss.str());
    }
    addr_list.clear();
}

bool analyze_context::entry_of(const string& name, variable_desc& desc)
{
    symbol_entry entry = sym_table_.find_entry(name);
    if (!sym_table_.valid_entry(entry))
        return false;
    desc = entry->second;
    return true;
}

void analyze_context::flush()
{
    path p(o_path_);
    bf::ofstream fout(p);
    if(!fout)
    {
        cerr << "File " << p.c_str() << " is not writable." << endl;
        return;
    }
    
    for (auto quad : codes_)
    {
        fout << quad << std::endl;
    }
}

