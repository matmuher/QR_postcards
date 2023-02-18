#pragma once

#include "Tokenizer.hpp"
#include "Objects.hpp"

class ParseNode;
using children_iter = std::vector<ParseNode*>::const_iterator;

// To express syntax constructions,
// that cant be straightly be got from source string
enum class ParseType
{
    // [semantic]
    TokenType, // node has one completely corresponding token
    Senteniel, // root
    AccumSizeScale,

    // [utility]
    Position,
    Number,

    Unknown
};

static void print_enum(std::ostream& cout, const char* token_type)
{
    const char* skipper = token_type;

    while(*skipper != ':') ++skipper;

    // skip "::"
        ++skipper;
        ++skipper;

    cout << skipper;
}

#define PRINT_ENUM(enum_value)                      \
                case enum_value:                    \
                    {print_enum(cout, #enum_value); \
                    break;}

std::ostream& operator<< (std::ostream& cout, ParseType type)
{
    cout << '[';
    switch (type)
    {
        PRINT_ENUM(ParseType::TokenType); // node has one completely corresponding token
        PRINT_ENUM(ParseType::Senteniel); // root
        PRINT_ENUM(ParseType::AccumSizeScale);

        // [utility]
        PRINT_ENUM(ParseType::Position);
        PRINT_ENUM(ParseType::Number);

        PRINT_ENUM(ParseType::Unknown);

        default:
            cout << "Unknown";
            break;
    }
    cout << ']';

    return cout;
}
#undef PRINT_ENUM

class ParseNode
{
    ParseType _type = ParseType::Unknown;

    const Token* const _token = nullptr; // token that is associated with this node

    // ParseNode* _neighbour;
    std::vector<ParseNode*> children;

public:

    ParseNode(ParseType type, const Token* token)
    :
        _type{type},
        _token{token} {};

    ParseNode(ParseType type) : ParseNode{type, nullptr} {}
    ParseNode(const Token* token) : ParseNode{ParseType::TokenType, token} {}

    // void addNeighbour(ParseNode* neighbour) { _neighbour = neighbour; }
    void addChild(ParseNode* child) { children.push_back(child); }

    void addChild(ParseType type, const Token* child_token)
        { children.push_back(new ParseNode{type, child_token}); }

    // ParseNode* getNeighbour() const { return _neighbour; }
    children_iter childrenBegin() const { return children.cbegin(); }
    children_iter childrenEnd() const { return children.end(); }

    TokenType token_type() const { return _token->type(); }
    ParseType parse_type() const { return _type; }

    // to easily get token-specific information (like text or number)
    const Token& token() const { return *_token; }
};

class PositionNode : public ParseNode
{
    int _x;
    int _y;

public:

    PositionNode(int x, int y)
    :
        ParseNode{ParseType::Position},
        _x{x},
        _y{y} {}

    int x() const { return _x; }
    int y() const { return _y; }
};


class NumberNode : public ParseNode
{
    int _num;

public:

    NumberNode(int num)
    :
        ParseNode{ParseType::Number},
        _num{num} {}

    int num() const { return _num; }
};

class Parser
{
    std::vector<Object> objects;

    const std::deque<Token*>& _token_que;
    std::deque<Token*>::const_iterator walker;
    const std::deque<Token*>::const_iterator end;

public:

    Parser(const std::deque<Token*>& token_que)
    :
        _token_que{token_que},
        walker{_token_que.begin()},
        end{_token_que.end()} {}

    bool require(TokenType required_token)
    {
        if (walker == end)
        {
            std::cout << "[info] end of token list\n";
            return false;
        }

        Token* cur_token = *walker;
        if (cur_token->type() != required_token)
        {
            std::cout << "[warn] wanted " << required_token << '\n'
                      << "got " << *(*walker) << "\n\n";
            return false;
        }
        else
            std::cout << "[info] got" << required_token << "\n\n";

        return true;
    }

    const Token* grab()
        {   return *walker++; }

   ParseNode* getSketch()
   {
        ParseNode* Sketch = new ParseNode(ParseType::Senteniel); // senteniel node

        for (ParseNode* new_child = nullptr; (new_child = getObj()) ;)
        {
            Sketch->addChild(new_child);
        }

        return Sketch;
   }
    
    #define REQUIRE(token_type) if (!require(token_type))                                        \
                                {   std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << '\n'; \
                                    return nullptr;}
   ParseNode* getObj()
   {
        std::cout << "getObj()\n";

        REQUIRE(TokenType::ObjectType);
        ParseNode* obj = new ParseNode{grab()};

        ParseNode* size_scale = getSizeScale();
        obj->addChild(size_scale);

        obj->addChild(getPos());

        for (ParseNode* prop; prop = getProp();)
        {
            obj->addChild(prop);
        }

        return obj;
   }

   ParseNode* getSizeScale()
   {
    std::cout << "getSizeScale()\n";

    int size = 0;

    for (; require(TokenType::SizeScale); grab())
        ++size;

    ParseNode* size_scale = new ParseNode{ParseType::AccumSizeScale};
    ParseNode* size_node  = new NumberNode{size};
    size_scale->addChild(size_node); // cringe

    return size_scale;
   }

    ParseNode* getPos()
    {
        std::cout << "getPos()\n";
        // [get a position] e.g. "[10,10]"

        REQUIRE(TokenType::LBrace); grab(); // '['

        REQUIRE(TokenType::Number); // '10'
            const Token* x = grab();

        REQUIRE(TokenType::Comma); grab(); // ','

        REQUIRE(TokenType::Number); // '10'
            const Token* y = grab();

        REQUIRE(TokenType::RBrace); grab(); // ']'

        ParseNode* position = new PositionNode(dynamic_cast<const QualifyToken*>(x)->specific(),
                                               dynamic_cast<const QualifyToken*>(y)->specific());

        return position;
    }

    ParseNode* getProp()
    {
        std::cout << "getProp()\n";

        REQUIRE(TokenType::LCurl); grab();

        REQUIRE(TokenType::Property); const Token* token = grab(); // get propery type out of here
        // grab property type

        REQUIRE(TokenType::Assign); grab();

        const Token* arg; // is init by default?

        if (require(TokenType::Number))
            arg = grab();
        else if (require(TokenType::Color))
            arg = grab();
        else
            return nullptr;

        REQUIRE(TokenType::SemiColon); grab();
        REQUIRE(TokenType::RCurl); grab();

        ParseNode* prop = new ParseNode{ParseType::TokenType, token};
        prop->addChild(ParseType::TokenType, arg);

        return prop;
    }


    std::vector<Object> parse()
    {   // exceptions? sequence of if's doesnt seem good

        // pine
        require(TokenType::ObjectType);

        // (!) cycle it
        require(TokenType::SizeScale);

        // [x,y]
        require(TokenType::LBrace);
        require(TokenType::Number);
        require(TokenType::Comma);
        require(TokenType::Number);
        require(TokenType::RBrace);

        // {
        require(TokenType::LCurl);

        // (!) cycle it
        // property = arg;
        require(TokenType::Property);
        require(TokenType::Assign);
        require(TokenType::Color);
        require(TokenType::SemiColon);

        // }
        require(TokenType::RCurl);

        std::cout << "[info] parsing is completed";
        return objects;
    }

    #undef REQUIRE
};

