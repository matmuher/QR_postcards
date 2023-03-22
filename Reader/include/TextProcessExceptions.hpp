#pragma once

#include <stdexcept>
#include <string>

class tokenize_error : public std::logic_error
{
public:
    
    tokenize_error(const char* msg) : std::logic_error{msg}
    {}

    const char* what() const noexcept override
    {
        
    }
};