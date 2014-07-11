#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include <string>

using std::string;

class quadruple
{
    int addr_;
    string op_;
    string arg1_;
    string arg2_;
    string dest_;

public:
    quadruple(int addr = 0, const string &op = "",
              const string &arg1 = "", const string &arg2 = "",
              const string &dest = "");
    
    string str() const;
    
    int addr() const { return addr_; }
    void addr(int addr) { addr_ = addr; }
    
    string oper() const { return op_; }
    void oper(const string &oper) { op_ = oper; }
    
    string arg1() const { return arg1_; }
    void arg1(const string &arg1) { arg1_ = arg1; }
    
    string arg2() const { return arg2_; }
    void arg2(const string &arg2) { arg2_ = arg2; }
    
    string dest() const { return dest_; }
    int dest_as_int() const;
    void dest(const string &dest) { dest_ = dest; }
    
};

std::ostream& operator<<(std::ostream& os, const quadruple& quad);
#endif // QUADRUPLE_H