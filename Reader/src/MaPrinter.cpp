#include <MaPrinter.hpp>

MaPrinter error_printer{"[error] "}, info_printer{"[info] "}, printer{""};

template<>
char get_print_spec<int>()
    {return 'd';}

template<>
char get_print_spec<const char*>()
    {return 's';}
    
template<>
char get_print_spec<float>()
    {return 'f';}
    
template<>
char get_print_spec<char>()
    {return 'c';}
