#ifndef OPT_BASE_H
#define OPT_BASE_H

#include "analyzer/non_terminal.h"
#include "lex/tkstream.h"

/**
 * Base class for all non-terminal which accepts empty string
 */
template <typename NtType>
class opt_base : public non_terminal
{
public:
    opt_base()
    {
        act_nt_ = new NtType;
    }
    
    virtual ~opt_base()
    {
        if (nullptr != act_nt_) delete act_nt_;
    }
    
    virtual bool parse(tkstream& input, analyze_context& context) override
    {
        if (!non_terminal::parse(input, context)) {
            return false;
        }
        
        bool res = true;
        empty_ = true;
        if (act_nt_->can_accept(input.peek())) {
            if (!act_nt_->parse(input, context)) {
                res = false;
                goto exit;
            }
            empty_ = false;
        }
        
    exit:
        return res;
    }
    
    virtual bool accept_empty() override
    {
        return true;
    }
    
    virtual bool can_accept(token cur_tk) override
    {
        return true;
    }
    
    bool empty()
    {
        return empty_;
    }
    
    NtType* actual_nt()
    {
        return act_nt_;
    }
private:

    bool empty_;
    NtType* act_nt_;
};
#endif // OPT_BASE_H