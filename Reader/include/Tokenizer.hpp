#pragma once

#include <iostream>
#include <unordered_map> // for fast tokenizing
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <iomanip> // cout hex
#include <Tokens.hpp>
#include <MaPrinter.hpp>

class Tokenizer
{
private: // [tokens list]

    const std::unordered_map<text_type, ObjectType> objects
    {
        {"pine",    ObjectType::Pine},
        {"star",    ObjectType::Star},
        {"gift",    ObjectType::Gift},
        {"congrat", ObjectType::Congratulation}
    };

    const std::unordered_map<text_type, PropertyType> props
    {
        {"color",   PropertyType::Color},
        {"size",    PropertyType::Size},
        {"intense", PropertyType::Intensity}
    };

    const std::unordered_map<text_type, ColorType> colors
    {
        {"red",     ColorType::Red},
        {"blue",    ColorType::Blue},
        {"yellow",  ColorType::Yellow},
        {"white",   ColorType::White},
        {"violet",  ColorType::Violet}
    };

private: // [stuff for error processing]

    int cur_line_id = 1;
    text_type::const_iterator cur_line_beg;

    std::map<int, text_type::const_iterator> source_lines;

private: // [stuff for processing source]

    text_type _src = "";
    text_type::const_iterator walker;
    text_type::const_iterator src_beg;
    text_type::const_iterator src_end;
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

    Token* dig_line();

    Token* dig_word();

    Token* dig_punct();

    Token* dig_number();

// [formatting]

    void skip_whites();

public:

// [error report]

    void print_line(std::ostream& cout, int line_id) const
    {
        cout << line_id << ":\n";

        text_type::const_iterator line_beg = source_lines.at(line_id);

        cout << '\t';
        for (auto it = line_beg; it != src_end && *it != '\n'; ++it)
        {
            std::cout << *it;
        }

        cout << '\n';
    }

    void print_anchor(std::ostream& cout, int line_id, text_type::const_iterator anchor) const
    {
        text_type::const_iterator line_beg = source_lines.at(line_id); 

        cout << '\t';
        for (auto it = line_beg; it != src_end && *it != '\n'; ++it)
        {
            if (it != anchor)
                cout << ' ';
            else
                cout << '^';
        } 
    }

public:

// [ctor]

    Tokenizer(const text_type& src)
    :
        _src{src},
        walker{_src.cbegin()},
        src_beg{walker},
        src_end{_src.end()}
    {}

    Tokenizer() {}

// [tokenize]

    const std::deque<Token*>& tokenize();

// [set]

    void initialize(const text_type& src)
    {
        _src = src;
        walker = _src.cbegin();

        src_beg = walker;
        src_end = _src.cend();

        token_que.clear();
        source_lines.clear();

        cur_line_id = 1;
        cur_line_beg = src_beg;

        source_lines[cur_line_id] = src_beg;
    }

// [get]

    const std::deque<Token*>& get_tokens() const { return token_que; }
    const std::map<int, text_type::const_iterator>& get_source_lines() const { return source_lines; }

// [dtor]

    ~Tokenizer() { for (Token* token_ptr : token_que) delete token_ptr; }
};

// [print]

std::ostream& operator<< (std::ostream& cout, const Tokenizer& tokenizer);

std::ostream& operator<< (std::ostream& cout, TokenType type);

std::ostream& operator<< (std::ostream& cout, Token token);
