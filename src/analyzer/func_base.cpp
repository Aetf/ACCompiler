#include "analyzer/func_base.h"

func_base::func_base()
{
    entry_ = -1;
}

bool func_base::accept_empty()
{
    return false;
}

void func_base::name(const string& new_name)
{
    name_ = new_name;
}

std::string func_base::name() const
{
    return name_;
}

void func_base::entry(int new_entry)
{
    entry_ = new_entry;
}

int func_base::entry() const
{
    return entry_;
}

vector< func_arg >& func_base::args() const
{
    return args_;
}
