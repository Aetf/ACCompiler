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
        ostringstream oss1, oss2;
        oss1 << "tests/case" << ncase << ".acc";
        oss2 << "tests/case" << ncase << ".acc.intm";
        path pin(oss1.str());
        
        
        cout << "Compiling file " << pin.c_str() << endl;
        
        bf::ifstream fin(pin);
        if(!fin)
        {
            cerr << "File " << pin.c_str() << " can't be accessed." << endl;
            return -1;
        }
        
        tkstream tkz(fin);
        tkz.on_lex_exception([&](lex_exception& ex){
            cerr << pin.filename() << ":" << ex.to_string() << endl;
        });
        
        starter S;
        analyze_context context(oss1.str(), oss2.str());
        
        if (S.parse(tkz, context)) {
            // output the intermidiate code
            context.flush();
        }
        cout << context.to_statistic_str() << endl << endl;
    }
    return 0;
}
