#ifndef LIST_BASE_H
#define LIST_BASE_H

#include <vector>
#include "analyzer/non_terminal.h"

using std::vector;

class list_more_base;

/**
 * Base class for all list related non_terminals
 */
class list_base : non_terminal
{
public:
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool accept_empty() override;
    
    /**
     * Parse the single part of a list
     */
    virtual bool parse_single(tkstream& input, analyze_context& context) = 0;
    
    /**
     * Create a non_terminal to parse the list_more part
     * @return a new allocated non_terminal
     */
    virtual list_more_base* create_list_more() = 0;
    
    /**
     * All list item in this list
     * Note: this content is filled only after the parse() method is succeed.
     */
    vector<non_terminal>& items() const;
    
private:
    vector<non_terminal> items_;
};

class list_more_base : non_terminal
{
public:
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool accept_empty() override ;
    
    /**
     * Create a non_terminal to parse the seperator part
     * @return a new allocated non_terminal
     */
    virtual list_more_base* create_sep() = 0;
    
    /**
     * Create a non_terminal to parse the list part
     * @return a new allocated non_terminal
     */
    virtual list_base* create_list() = 0;
    
    /**
     * All list item in this list
     * Note: this content is filled only after the parse() method is succeed.
     */
    vector<non_terminal>& items() const;
    
private:
    vector<non_terminal> items_;
};

#endif