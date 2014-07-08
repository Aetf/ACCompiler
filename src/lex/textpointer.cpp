#include <cstdio>
#include <string>

#include "lex/textpointer.h"
#include "lex/token.h"

text_pointer::text_pointer()
{
    clear();
}

text_pointer::text_pointer(int line, int col)
{
    this->col = col;
    this->line = line;
    state = -1;
    atlast = false;
}

void text_pointer::clear()
{
    col = 0;
    line = 1;
    state = -1;
    atlast = false;
}

bool text_pointer::step(int ch)
{
    if(ch == EOF)
    {
        return true;
    }
    
    if(islast())
    {
        atlast = false;
        col = 0;
        line++;
        state = -1;
    }
    
    col++;
    if(token::islinefeedchar(ch))
    {
        unsigned int nxt = token::curr_linefeed().find_first_of(ch);
        if(state + 1 == nxt)
        {
            state = nxt;
            if(state == token::curr_linefeed().size() - 1)
            {
                // got linefeed!
                atlast = true;
                return atlast;
            }
        }
        else
        {
            state = -1;
        }
    }
    return false;
}

bool text_pointer::islast()
{
    return atlast;
}