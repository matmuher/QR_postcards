#pragma once

#include <stdexcept>
#include <string>
#include <Tokenizer.hpp>

struct tokenize_error : public std::logic_error
{
    text_type::const_iterator _anchor;
    int _line_id;

    tokenize_error(const char* msg, int line_id, text_type::const_iterator anchor)
    :
        std::logic_error{msg},
        _line_id{line_id},
        _anchor{anchor}
    {}
};