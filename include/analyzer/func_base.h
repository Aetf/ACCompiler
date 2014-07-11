#ifndef FUNC_BASE_H
#define FUNC_BASE_H

#include <string>
#include <vector>

#include "analyzer/non_terminal.h"

using std::string;
using std::vector;

class token;

struct func_arg
{
    func_arg(const string& n = "", const string& t = "") 
        :name(n), type(t) { }
    string name;
    string type;
};

/**
 * Base class for all function related non_terminals
 */
class func_base : public non_terminal
{
public:
    virtual ~func_base() {}
    func_base();
    
    /**
     * Get and set the name of the function.
     */
    string name() const;
    void name(const string& new_name);
    
    /**
     * Get and set the entry point of the function.
     */
    int entry() const;
    void entry(int new_entry);
    
    /**
     * Get the vector of function arguments
     */
    vector<func_arg>& args();
protected:
    
private:
    vector<func_arg> args_;
    string name_;
    int entry_;
};


/*
 * Classes derived from func_base
 */

class func_def : public func_base
{
public:
    virtual ~func_def() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    bool only_sign() const { return sign_; }
private:
    bool sign_;
};

class func_def_part : public func_base
{
public:
    func_def_part(const string &name, const string &ret_type)
        : name_(name), ret_type_(ret_type) { }
    virtual ~func_def_part() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
    
    bool only_sign() const { return sign_; }
    
    int entry() const { return entry_; }
private:
    bool sign_;
    int entry_;
    string name_;
    string ret_type_;
};

class func_sign : public func_base
{
public:
    virtual ~func_sign() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};

/*
class func_call : public func_base
{
public:
    virtual ~func_call() {}
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool can_accept(token cur_tk) override;
};
*/

#endif