#pragma once

#include <Objects.hpp>
#include <Tokens.hpp>
#include <sstream>

template<class T>
std::ostream& print_enum (std::ostream& cout, T enum_val)
{
    return cout << '[' << magic_enum::enum_name(enum_val) << ']';
}

template<class T>
std::string str_enum(T enum_val)
{
    std::stringstream tmp_stream;
    tmp_stream << magic_enum::enum_name(enum_val);
    return tmp_stream.str();
}
