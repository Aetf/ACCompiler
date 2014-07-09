#ifndef FUNC_BASE_H
#define FUNC_BASE_H

#include <string>
#include <vector>

#include "analyzer/non_terminal.h"

using std::string;
using std::vector;

struct func_arg;
class token;

/**
 * Base class for all function related non_terminals
 */
class func_base : non_terminal
{
public:
    func_base();
    virtual bool accept_empty() override;
    
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
    vector<func_arg>& args() const;
protected:
    
private:
    vector<func_arg> args_;
    string name_;
    int entry_;
};

struct func_arg
{
    string name;
    string type;
};
#endif