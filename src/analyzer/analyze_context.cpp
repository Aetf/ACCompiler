#include <iostream>
#include "analyzer/analyze_context.h"
#include "lex/token.h"

std::ostream& operator<<(std::ostream& os, const token& tk)
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

void analyze_context::on_error(token cur_tk)
{
    std::cout << "Error: "<< cur_tk << std::endl;
}

int analyze_context::next_address() const
{
    return 0;
}

int analyze_context::generate()
{
    return 0;
}
