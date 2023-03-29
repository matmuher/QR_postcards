#pragma once

#include <stdexcept>
#include <string>
#include <Tokenizer.hpp>

struct tokenize_error : public std::logic_error
{
    int line_id;
    text_type::const_iterator anchor;

    tokenize_error(const char* msg, int _line_id, text_type::const_iterator _anchor)
    :
        std::logic_error{msg},
        line_id{_line_id},
        anchor{_anchor}
    {}
};

struct parse_error : public std::logic_error
{
    TokenType required_type;
    TokenType got_type;

    int line_id;
    text_type::const_iterator anchor;

public:

    parse_error(TokenType _required_type, TokenType _got_type,
                int _line_id, text_type::const_iterator _anchor)
    :
        std::logic_error{"Syntax error"},
        required_type{_required_type},
        got_type{_got_type},
        line_id{_line_id},
        anchor{_anchor} {}
};