#pragma once

#include "Tokenizer.hpp"
#include "Objects.hpp"

class ParseTree;
using children_iter = std::vector<ParseTree*>::const_iterator;

class ParseTree
{
    const Token* const _token; // token that is associated with this node

    // ParseTree* _neighbour;
    std::vector<ParseTree*> children;

public:

    ParseTree(const Token* token) : _token{token} {};

    // void addNeighbour(ParseTree* neighbour) { _neighbour = neighbour; }
    void addChild(ParseTree* child) { children.push_back(child); }
    void addChild(const Token* child_token) { children.push_back(new ParseTree{child_token}); }

    // ParseTree* getNeighbour() const { return _neighbour; }
    children_iter childrenBegin() const { return children.cbegin(); }
    children_iter childrenEnd() const { return children.end(); }

    TokenType type() const { return _token->type(); }

    // to easily get token-specific information (like text or number)
    const Token& token() const { return *_token; }
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
            std::cout << "[info: require] end of token list";
            return false;
        }

        Token* cur_token = *walker;
        if (cur_token->type() != required_token)
        {
            std::cout << "[error] requirement missing for " << *(*walker) << '\n';
            return false;
        }

        return true;
    }

    const Token* grab()
        {   return *walker++; }

   ParseTree* getSketch()
   {
        ParseTree* Sketch = new ParseTree(nullptr); // senteniel node

        for (ParseTree* new_child = nullptr; new_child = getObj();)
        {
            Sketch->addChild(new_child);
        }

        return Sketch;
   }
    
    #define REQUIRE(token_type) if (!require(TokenType::ObjectType)) \
                                    return nullptr;
   ParseTree* getObj()
   {
        REQUIRE(TokenType::ObjectType);
        ParseTree* obj = new ParseTree{grab()};

        if (ParseTree* size_scale = nullptr; size_scale = getSizeScale())
            obj->addChild(size_scale);

        obj->addChild(getPos());

        for (ParseTree* prop; prop = getProp();)
        {
            obj->addChild(prop);
        }

        return obj;
   }

   ParseTree* getSizeScale()
   {
    int size = 0;

    for (; require(TokenType::SizeScale); grab())
        ++size;

    ParseTree* size_scale = new ParseTree{nullptr};
    size_scale->addChild(reinterpret_cast<const Token*>(size)); // cringe

    return size_scale;
   }

    ParseTree* getPos()
    {
        // [get a position] e.g. "[10,10]"

        REQUIRE(TokenType::LBrace); grab(); // '['

        REQUIRE(TokenType::Number); // '10'
            const Token* x = grab();

        REQUIRE(TokenType::Comma); grab(); // ','

        REQUIRE(TokenType::Number); // '10'
            const Token* y = grab();

        REQUIRE(TokenType::RBrace); grab(); // ']'

        ParseTree* position = new ParseTree(nullptr); // add som marker for properties
        position->addChild(x);
        position->addChild(y);

        return position;
    }

    ParseTree* getProp()
    {
        REQUIRE(TokenType::LCurl); grab();

        REQUIRE(TokenType::Property); grab();
        // grab property type

        REQUIRE(TokenType::Assign); grab();

        const Token* arg; // is init by default?

        if (require(TokenType::Number))
            arg = grab();
        else if (require(TokenType::Color))
            arg = grab();
        else
            return nullptr;

        REQUIRE(TokenType::RCurl);

        ParseTree* prop = new ParseTree{nullptr};
        prop->addChild(arg);

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

