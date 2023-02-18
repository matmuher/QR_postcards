#pragma once

#include <string>

using text_type = std::string;

enum class ObjectType
{
    Pine,
    Star
};

enum class ColorType
{
    Red,
    Green,
    Blue
};

enum class PropertyType
{
    Size,
    Color
};

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

class QualifyToken : public Token
{
    TokenType _general_type;
    int _specific_type;

public:

    QualifyToken(TokenType general_type, int  specific_type,
                 text_type::const_iterator start, text_type::const_iterator end)
    :
        Token(general_type, start, end),
        _general_type(general_type),
        _specific_type(specific_type) {}

    virtual TokenType   general()   const { return _general_type; }
    virtual int         specific()  const { return _specific_type; }

    virtual std::ostream& print (std::ostream& cout) override;
};
