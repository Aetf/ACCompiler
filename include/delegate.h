#ifndef DELEGATE_H
#define DELEGATE_H

#include <functional>
#include <vector>

// general case
template<typename R, typename... Args>
class delegate
{
public:
    template<typename U>
    delegate& operator += (const U &func)
    {
        funcs.push_back(std::function<R(Args...)>(func));
        return *this;
    }
    
    std::vector<R> operator () (Args... params)
    {
        std::vector<R> ret;
        for (auto f : funcs)
        {
            ret.push_back(f(params...));
        }
        return ret;
    }
private:
    std::vector<std::function<R(Args...)>> funcs;
};

// specialization when return type is void
template<typename... Args>
class delegate<void, Args...>
{
public:
    template<typename U>
    delegate& operator += (const U &func)
    {
        funcs.push_back(std::function<void(Args...)>(func));
        return *this;
    }
    
    void operator () (Args... params)
    {
        for (auto f : funcs)
        {
            f(params...);
        }
    }
private:
    std::vector<std::function<void(Args...)>> funcs;
};

#endif // DELEGATE_H