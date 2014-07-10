#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "analyzer/analyze_context.h"
#include "analyzer/non_terminal.h"
#include "lex/lexexception.h"
#include "lex/token.h"
#include "lex/tkstream.h"

namespace bf = boost::filesystem;

using namespace std;
using bf::path;

int main(int argc, char **argv)
{
    for (int ncase = 1; ncase <= 6; ncase++)
    {
        ostringstream oss;
        oss << "tests/case" << ncase << ".acc";
        path p(oss.str());
        
        cout << "Compiling file " << p.c_str() << endl;
        
        bf::ifstream fin(p);
        if(!fin)
        {
            cerr << "File " << p.c_str() << " can't be accessed." << endl;
            return -1;
        }
        
        tkstream tkz(fin);
        tkz.on_lex_exception([&](lex_exception& ex){
            cerr << p.filename() << ":" << ex.to_string() << endl;
        });
        
        starter S;
        analyze_context context;
        
        S.parse(tkz, context);
        
        cout << "File " << p.c_str() << " done." << endl;
    }
    return 0;
}
