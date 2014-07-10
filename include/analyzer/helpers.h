#ifndef HELPERS_H
#define HELPERS_H

#define parse_use(nt) \
    if (!(nt)->can_accept(input.peek())) { \
        context.on_error(input.peek()); \
        res = false; \
        goto exit; \
    } \
    if (!(nt)->parse(input, context)) { \
        res = false; \
        goto exit; \
    }
        
#define advance_if(tkid) \
    if (input.peek().id() != (tkid)) { \
        context.on_error(input.peek()); \
        res = false; \
        goto exit; \
    } \
    input.advance();
            
#define extract_to(tkid, tk) \
    if (input.peek().id() != (tkid)) { \
        context.on_error(input.peek()); \
        res = false; \
        goto exit; \
    } \
    input >> (tk);
    
#endif // HELPERS_H