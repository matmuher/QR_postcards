#pragma once

#include <string>

using text_type = std::string;

enum class TokenType
{
    ObjectType,
    Property,
    Color,
    SizeScale,

    // [brackets]
        RBrace,
        LBrace,
        RCurl,
        LCurl,
        RRound,
        LRound,

    // [delimiters]
        Comma,
        SemiColon,

    Assign,
    Number,
    Unknown
};

class Token
{
    TokenType _type;

    const text_type::const_iterator _start;
    const text_type::const_iterator _end;

public:

// [ctor]

    Token(TokenType type, text_type::const_iterator start, text_type::const_iterator end)
    :
        _type{type},
        _start{start},
        _end{end} {}

// [get]

    TokenType type() const { return _type; }

    text_type::const_iterator start() const { return _start; }
    text_type::const_iterator end() const { return _end; }

// [print]

    // made virtual to allow special tokens print themselves in a clever way
    virtual std::ostream& print (std::ostream& cout);

// [dtor]

    virtual ~Token() {};
};

class TokenNumber : public Token
{
    int _num;

public:

// [ctor]

    TokenNumber(int num, text_type::const_iterator start, text_type::const_iterator end)
    :
        Token{TokenType::Number, start, end},
        _num{num} {}

// [get]

    int num() const { return _num; }

// [print]

    std::ostream& print (std::ostream& cout) override;
};

using pixel_color = unsigned int;

class TokenColor : public Token
{
    pixel_color _color;

public:

    TokenColor(pixel_color color, text_type::const_iterator start, text_type::const_iterator end)
    :
        Token{TokenType::Color, start, end},
        _color{color} {}

    pixel_color color() const { return _color; }

    std::ostream& print (std::ostream& cout) override;
};
