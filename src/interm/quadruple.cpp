#include <sstream>

#include "interm/quadruple.h"

using std::ostringstream;
using std::istringstream;

quadruple::quadruple(int addr, const std::string& op,
                     const std::string& arg1, const std::string& arg2, const std::string& dest)
    :addr_(addr), op_(op), arg1_(arg1), arg2_(arg2), dest_(dest)
{ }

string quadruple::str() const
{
    ostringstream oss;
    oss << addr_ << " ("
                << (op_.empty() ? " - " : op_)
                << (arg1_.empty() ? " - " : arg1_)
                << (arg2_.empty() ? " - " : arg2_)
                << (dest_.empty() ? " - " : dest_)
                <<")";
    return oss.str();
}

int quadruple::dest_as_int() const
{
    istringstream iss(dest_);
    int d = -1;
    iss >> d;
    return d;
}

std::ostream& operator<<(std::ostream& os, const quadruple& quad)
{
    return os << quad.str();
}
