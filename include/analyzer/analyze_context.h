#ifndef CONTEXT_H
#define CONTEXT_H

class analyze_context
{
public:
    void on_error();
    
    int generate();
    
    int next_address() const;
    
};

#endif