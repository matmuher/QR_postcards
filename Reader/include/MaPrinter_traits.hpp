#pragma once

#include <iostream>

template<class T>
char get_print_spec()
{
    std::cout << "[MaPrinter_traits] specifier is unknown for " << __PRETTY_FUNCTION__ << '\n';
    return 0;
}
