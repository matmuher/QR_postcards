#pragma once

#include <map>
#include <vector>
#include <magic_enum.hpp>

#include <Tokenizer.hpp>
#include <Objects.hpp>

class ParseNode;

using children_iter = std::vector<ParseNode*>::const_iterator;
using PropPair = std::pair<PropertyType, const ParseNode*>;
using ObjectPair = std::pair<ObjectType, const ParseNode*>;

enum class ParseType
{
    // [so far left for backward compability]

        // [semantic]
        TokenType, // node has one completely corresponding token
        AccumSizeScale,

        // [utility]
        Position,

    // [new gen node types]
    Senteniel,
    Object,
    Property,
    Number,
    Unknown
};

std::ostream& operator<< (std::ostream& cout, ParseType type)
{
    return cout << '[' << magic_enum::enum_name(type) << ']';
}

void print_indent(std::ostream& cout, int indent)
    {while (indent--) cout << ' ';}

class ParseNode
{
    ParseType _type = ParseType::Unknown;

    std::vector<ParseNode*> children;

public:

    ParseNode(ParseType type) : _type{type} {};

    void addChild(ParseNode* child) { children.push_back(child); }
    void addChild(ParseType type)   { children.push_back(new ParseNode{type}); }

    children_iter childrenBegin() const { return children.cbegin(); }
    children_iter childrenEnd()   const { return children.end(); }

    ParseType type() const { return _type; }

    virtual ~ParseNode() {} // delete

    virtual void print(std::ostream& cout, int indent) const
    {
        print_indent(cout, indent);
        cout << magic_enum::enum_name(_type);

        auto st = children.begin();
        auto end = children.end();

        while (st != end)
        {   
            cout << '\n';
            (*st)->print(cout, indent+4);
            cout << ' ';
            ++st;
        }
    }
};

class ObjectNode : public ParseNode
{
    ObjectType _object_type;

public:
    // to get children in associative way
    std::map<PropertyType, const ParseNode*> props;

    // to get subobjects in assciative way
    // object can have several subobjects of the same type
    std::multimap<ObjectType, ObjectNode*> subobjects;

    ObjectNode(ObjectType object_type) : ParseNode(ParseType::Object), _object_type{object_type} {}

    ObjectType object_type() const { return _object_type; }

    virtual void print(std::ostream& cout, int indent) const
    {
        print_indent(cout, indent);
        cout << magic_enum::enum_name(_object_type) << '\n';

        for (auto& elem : props)
        {
            print_indent(cout, indent+4);
            cout << magic_enum::enum_name(elem.first) << ":\n";
            elem.second->print(cout, indent+8);
            cout << '\n';
        }

        for (auto& elem : subobjects)
        {
            (elem.second)->print(cout, indent+4);
            cout << '\n';
        }
    }
};

class NumberNode : public ParseNode
{
    int _num;

public:

    NumberNode(int num) : ParseNode{ParseType::Number}, _num{num} {}

    int num() const { return _num; }

    virtual void print(std::ostream& cout, int indent) const
    {
        ParseNode::print(cout, indent);
        cout << ' ' << _num;
    }
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

    // TODO syntax errors handling
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
                      << "       got " << *(*walker) << "\n\n";
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

        for (ParseNode* new_object = nullptr; (new_object = getObj()) ;)
            {Sketch->addChild(new_object);}

        return Sketch;
   }
    
    #define REQUIRE(token_type) if (!require(token_type))                                        \
                                {   std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << '\n'; \
                                    return nullptr;}
   ParseNode* getObj()
   {
        std::cout << "getObj()\n";

        REQUIRE(TokenType::ObjectType);

            // TODO more clear way to code it
            const QualifyToken* token = dynamic_cast<const QualifyToken*>(grab());
            ObjectType obj_type = static_cast<ObjectType>(token->specific());
            ObjectNode* obj_node = new ObjectNode(obj_type); 

        PropPair size = getSize();
            obj_node->props[size.first] = size.second;

        if (PropPair pos = getPos(); pos.second)
            obj_node->props[pos.first] = pos.second;

        REQUIRE(TokenType::LCurl); grab();
        
        while(true)
        {
            if (PropPair prop_pair = getProp(); prop_pair.second)
            {
                obj_node->props[prop_pair.first] = prop_pair.second;
            }
            else if (ParseNode* new_object = getObj(); new_object)
            {
                obj_node->subobjects.insert({ObjectType::PineTop, dynamic_cast<ObjectNode*>(new_object)});
            }
            else
            {
                break;
            }
        }

        REQUIRE(TokenType::RCurl); grab();

        return obj_node;
   }

    #define REQUIRE(token_type) if(!require(token_type))                                         \
                            {                                                                \
                                std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << '\n'; \
                                return PropPair(PropertyType::Unknown, nullptr);             \
                            }

   PropPair getSize()
   {
    std::cout << "getSize()\n";

    int size = 0;

    for (; require(TokenType::SizeScale); grab())
        ++size;

    ParseNode* size_node = new ParseNode{ParseType::Property};
    size_node->addChild(new NumberNode{size});

    return PropPair(PropertyType::Size, size_node);
   }

    PropPair getPos()
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

        ParseNode* position_node = new ParseNode(ParseType::Property);
        position_node->addChild(new NumberNode(dynamic_cast<const QualifyToken*>(x)->specific()));
        position_node->addChild(new NumberNode(dynamic_cast<const QualifyToken*>(y)->specific()));

        return PropPair(PropertyType::Position, position_node);
    }

    PropPair getProp()
    {
        std::cout << "getProp()\n";

        REQUIRE(TokenType::Property);
        const QualifyToken* token = dynamic_cast<const QualifyToken*>(grab());
        PropertyType prop_type = static_cast<PropertyType>(token->specific());

        REQUIRE(TokenType::Assign); grab();

        ParseNode* arg_node = nullptr;

        if (require(TokenType::Number) || require(TokenType::Color))
        {
            auto token = dynamic_cast<const QualifyToken*>(grab());
            int arg_num = token->specific();
            arg_node = new NumberNode{arg_num}; 
        }
        else
        {
            return PropPair(PropertyType::Unknown, nullptr);
        }

        REQUIRE(TokenType::SemiColon); grab();

        ParseNode* prop_node = new ParseNode{ParseType::Property};
        prop_node->addChild(arg_node);

        return PropPair(prop_type, prop_node);
    }

    #undef REQUIRE
};

