#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <MaPrinter_traits.hpp>

static std::ostream& print_error(const char* errored_module = NULL, std::ostream& outs = std::cout)
{
    static std::ostream& _outs = outs;
    _outs << "\n[error";
    
    if (errored_module != NULL)
        outs << ": " << errored_module;
    
    outs << "] ";
    
    return outs;
}

class MaPrinter
{
    std::ostream& _outs;
    std::string _prolog_msg;
    bool is_prologue = true;
    
public:

    MaPrinter( const std::string& prolog_msg = "", std::ostream& outs = std::cout)
    :
        _outs{outs},
        _prolog_msg{prolog_msg} {}

    void ma_print(const char* str)
    {
        assert(str != nullptr);

        if (is_prologue)
        {
            _outs << _prolog_msg;
            is_prologue = false;
        }

        while(*str)
            putchar(*str++);
            
        is_prologue = true; // recover flag
    }
  
    #define CHECK_SPEC(spec)                                                                   \
        case spec:                                                                             \
            if (char arg_spec = get_print_spec<T>(); spec != arg_spec)                         \
                print_error("ma_print") << "format string specifier(%" << spec << ")"          \
                "and argument specifier (%" << arg_spec << ") dont match\n";                   \
            break;
                
    template<class T, typename... ArgsT>
    void ma_print(const char* str, T head, ArgsT... tail)
    {
        assert(str != nullptr);

        if (is_prologue)
        {
            _outs << _prolog_msg;
            is_prologue = false;
        }

        while(*str && *str != '%')
            putchar(*str++);
            
        ++str;
        switch(*str)
        {
            CHECK_SPEC('d');
            CHECK_SPEC('s');
            CHECK_SPEC('f');
            CHECK_SPEC('c');
            
            default:
                print_error("ma_print") << "format string specifier is not supported: %" << *str << '\n';
        }
            
        _outs << head;
        ++str;
        
        ma_print(str, tail...);
    }
    #undef CHECK_SPEC  
};

extern MaPrinter error_printer, info_pinter, printer;