#ifndef CODE_POS_H
#define CODE_POS_H

struct text_pointer
{
    int line;
    int col;
    
    text_pointer();
    
    text_pointer(int line, int col);
    
    /**
     * @Return true if the ch is the last char of a line.
     */
    bool step(int ch);
    
    /**
     * Reset to first line first column
     */
    void clear();
    
    /**
     * @Return whether points at the last character of a line.
     */
    bool islast();
    
private:
    unsigned int state;
    bool atlast;
};

#endif // CODE_POS_H