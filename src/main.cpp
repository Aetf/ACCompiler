#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "lex/lexexception.h"
#include "lex/token.h"
#include "lex/tokenizer.h"

namespace bf = boost::filesystem;

using namespace std;
using bf::path;

ostream& operator<<(ostream& os, const token& tk);

int main(int argc, char **argv)
{
    path p("test.txt");
    bf::ifstream fin(p);
    if(!fin)
    {
        cerr << "File " << p.c_str() << " can't be accessed." << endl;
        return -1;
    }
    
    tokenizer tkz(fin);
    tkz.on_lex_exception([&](lex_exception& ex){
        cerr << p.filename() << ":" << ex.to_string() << endl;
    });
    
    token tk;
    while(tkz >> tk)
    {
        if(tk.iscomment()) continue;
        
        cout << tk << endl;
        if(tk.iscontant())
        {
            cout << tk << endl;
        } else if (tk.isidentifier())
        {
            cout << tk << endl;
        }
    }
    
    return 0;
}

ostream& operator<<(ostream& os, const token& tk)
{
    os << "("
    << "id:" << tk.id() << ", "
    << "pos:" << tk.position().line << ":" << tk.position().col << ", ";
    
    if(tk.isreserved())
        os << "-";
    else
        os << tk.text();
    
    os << ")";
    return os;
}
