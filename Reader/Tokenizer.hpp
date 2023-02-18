#pragma once

#include <iostream>
#include <unordered_map> // for fast tokenizing
#include <vector>
#include <deque>
#include <iomanip> // cout hex
#include "Tokens.hpp"

class Tokenizer
{
private: // [tokens list]

    const std::unordered_map<text_type, ObjectType> objects
    {
        {"pine",    ObjectType::Pine},
        {"star",    ObjectType::Star}
    };

    const std::unordered_map<text_type, PropertyType> props
    {
        {"color",   PropertyType::Color},
        {"size",    PropertyType::Size}
    };

    const std::unordered_map<text_type, ColorType> colors
    {
        {"red",     ColorType::Red},
        {"green",   ColorType::Green},
        {"blue",    ColorType::Blue}
    };

private: // [stuff for processing source]

    const text_type& _src;
    text_type::const_iterator walker;
    const text_type::const_iterator src_end;
    std::deque<Token*> token_que;

// [token creators]

    // may be extended for more informable tokenizing
    Token* create_token(TokenType type,
                        text_type::const_iterator start,
                        text_type::const_iterator end);

    Token* create_token(int num,
                        text_type::const_iterator start,
                        text_type::const_iterator end);

    Token* create_token(TokenType general_type, int specific_type,
                        text_type::const_iterator start,
                        text_type::const_iterator end);

// [diggers]

    Token* dig_word();

    Token* dig_punct();

    Token* dig_number();

// [others]

    void skip_whites();

public:

// [ctor]

    Tokenizer(const text_type& src)
    :
        _src{src},
        walker{_src.cbegin()},
        src_end{_src.end()}
    {}

// [tokenize]

    void tokenize();

// [get]

    const std::deque<Token*>& get_tokens() const { return token_que; }

// [dtor]

    ~Tokenizer() { for (Token* token_ptr : token_que) delete token_ptr; }
};

// [print]

std::ostream& operator<< (std::ostream& cout, const Tokenizer& tokenizer);

std::ostream& operator<< (std::ostream& cout, TokenType type);

std::ostream& operator<< (std::ostream& cout, Token token);
