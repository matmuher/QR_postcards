#pragma once

#include <string>
#include <magic_enum.hpp>
#include <Objects.hpp>

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

    Plus,
    String,
    Assign,
    Number,
    Unknown
};

struct SourceLine;

class Token
{
    TokenType _type;

    const text_type::const_iterator _start;
    const text_type::const_iterator _end;

    int _line_id;

public:

// [ctor]

    Token(TokenType type, text_type::const_iterator start, text_type::const_iterator end)
    :
        _type{type},
        _start{start},
        _end{end},
        _line_id{-1} {}

// [set]

void set_src_line(int line_id) { _line_id = line_id; }

// [get]

    TokenType type() const { return _type; }

    text_type::const_iterator start() const { return _start; }
    text_type::const_iterator end() const { return _end; }

    int line_id() const { return _line_id; }

// [print]

    // made virtual to allow special tokens print themselves in a clever way
    virtual std::ostream& print (std::ostream& cout) const;

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

    virtual std::ostream& print (std::ostream& cout) const override;
};
