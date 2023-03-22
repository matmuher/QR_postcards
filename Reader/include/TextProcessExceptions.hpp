#pragma once

#include <stdexcept>
#include <string>
#include <Tokenizer.hpp>

struct tokenize_error : public std::logic_error
{
    text_type::const_iterator _anchor_it;

    tokenize_error(const char* msg, text_type::const_iterator anchor_it)
    :
        std::logic_error{msg},
        _anchor_it{anchor_it}
    {}
};