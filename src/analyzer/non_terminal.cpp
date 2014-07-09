#include "analyzer/analyze_context.h"
#include "analyzer/non_terminal.h"
#include "lex/tkstream.h"
#include "lex/token.h"

bool non_terminal::parse(tkstream& input, analyze_context& context)
{
    if(!input.peek().good() && !accept_empty()) {
        context.on_error();
        return false;
    }
    
    if(!can_accept(input.peek())) {
        context.on_error();
        return false;
    }
    return true;
}
