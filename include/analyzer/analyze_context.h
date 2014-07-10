#ifndef CONTEXT_H
#define CONTEXT_H

class token;

class analyze_context
{
public:
    void on_error(token cur_tk);
    
    int generate();
    
    int next_address() const;
    
    void flush();
    
};

#endif