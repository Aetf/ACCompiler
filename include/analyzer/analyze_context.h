#ifndef CONTEXT_H
#define CONTEXT_H

// #include "interm/quadruple.h"
#include <vector>
#include <string>

#include "analyzer/symbol_table.h"
#include "lex/textpointer.h"
#include "interm/quadruple.h"

using std::string;
using std::vector;

class token;

class analyze_context
{
public:
    analyze_context(const string &input_file, const string &output_file);
    
    void on_note(const string& msg, const text_pointer& pointer);
    void on_note(const string& msg);
    void on_error(token cur_tk);
    void on_error(const string &msg);
    void on_error(const string &msg, const text_pointer& pointer);
    void on_critical(const string &msg);
    void on_critical(const string &msg, const text_pointer& pointer);
    
    int generate(const string &op = "",
                 const string &arg1 = "", const string &arg2 = "",
                 const string &dest = "");
    int generate(const string &op = "",
                 int arg1 = 0, const string &arg2 = "",
                 const string &dest = "");
    int generate(const string &op = "",
                 const string &arg1 = "", int arg2 = 0,
                 const string &dest = "");
    int generate(const string &op = "",
                 const string &arg1 = "", const string &arg2 = "",
                 int dest = 0);
    int generate(const string &op = "",
                 const string &arg1 = "", int arg2 = 0,
                 int dest = 0);
    int generate(const string &op = "",
                 int arg1 = 0, const string &arg2 = "",
                 int dest = 0);
    int generate(const string &op = "",
                 int arg1 = 0, int arg2 = 0,
                 int dest = 0);
    
    int next_address() const;
    
    void back_patch(vector<int> &addr_list, int value);
    
    bool entry_of(const string& name, variable_desc& desc);
    
    func_entry curr_func() const;
    void enter_func(func_entry entry);
    void exit_func();
    
    int error_count() const { return error_cnt_; }
    int warning_count() const { return warning_cnt_; }
    string to_statistic_str() const;
    
    vector<quadruple>& codes() { return codes_; }
    
    symbol_table& table() { return sym_table_; }
    
    void flush();

private:
    
    string o_path_;
    string i_path_;
    vector<quadruple> codes_;
    symbol_table sym_table_;
    func_entry curr_func_;
    bool in_func_;
    
    int error_cnt_;
    int warning_cnt_;
};

#endif