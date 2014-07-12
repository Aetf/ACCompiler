#ifndef NON_TERMINAL_BASE_H
#define NON_TERMINAL_BASE_H

#include "lex/token.h"

class analyze_context;
class tkstream;

class non_terminal
{
public:
    virtual ~non_terminal() {}
    
    /**
     * Parse using this non_terminal
     * @param input the tokenstream of the input file.
     * @param context the compiler context.
     * @return true on succeed.
     */
    virtual bool parse(tkstream& input, analyze_context& context);
    
    /**
     * Whether the token is in this non_terminal's FIRST collection.
     * @return true if the cur_tk is found in the FIRST collection, otherwise false.
     */
    virtual bool can_accept(token cur_tk) = 0;
    
    /**
     * Whether this non_terminal accept empty string.
     * Default is false.
     * @return true if accept otherwise false.
     */
    virtual bool accept_empty();
};

#endif // NON_TERMINAL_BASE_H