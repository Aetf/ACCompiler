#ifndef OPT_BASE_H
#define OPT_BASE_H

#include "analyzer/non_terminal.h"

/**
 * Base class for all non-terminal which accepts empty string
 */
class opt_base : non_terminal
{
public:
    virtual bool parse(tkstream& input, analyze_context& context) override;
    virtual bool accept_empty() override;
    
    /**
     * Create the non_terminal used to parse the string.
     * @return an new allocated non_terminal
     */
    virtual non_terminal* create_non_terminal() = 0;
};

#endif // OPT_BASE_H