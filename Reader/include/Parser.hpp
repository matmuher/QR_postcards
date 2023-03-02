#pragma once

#include <map>
#include <vector>
#include <magic_enum.hpp>

#include <Tokenizer.hpp>
#include <Objects.hpp>

class ParseNode;
class PropertyNode;
class ObjectNode;

using children_iter = std::vector<ObjectNode*>::const_iterator;

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
{
    while (indent--) cout << ' ';
}

class ParseNode
{
protected:

    ParseType _type = ParseType::Unknown;

public:

    ParseNode(ParseType type) : _type{type} {};

    virtual ~ParseNode() {} // delete

    ParseType type() const { return _type; }

    virtual void print(std::ostream& cout, int indent) const
    {
        print_indent(cout, indent);
        cout << magic_enum::enum_name(_type);
    }
};

class PropertyNode
{
    PropertyType _type;

public:

    PropertyNode(PropertyType type)
    :
        _type{type} {}

    PropertyType type() const { return _type; }

    virtual void print(std::ostream& cout, int indent) const
    {
        print_indent(cout, indent);
        cout << '[' << _type << ']';
    }
};

class PositionNode : public PropertyNode
{
    int _x = -1;
    int _y = -1;

public:

    PositionNode(int x, int y)
    :
        PropertyNode{PropertyType::Position},
        _x{x},
        _y{y} {}

    int x() const { return _x; }
    int y() const { return _y; }

    void print(std::ostream& cout, int indent) const override
    {
        PropertyNode::print(cout, indent);
        cout << "x: " << _x << ", y: " << _y;
    }
};

class SizeNode : public PropertyNode
{
    int _size = -1;

public:

    SizeNode(int size)
    :
        PropertyNode{PropertyType::Size},
        _size{size} {}

    int size() const { return _size; }

    void print(std::ostream& cout, int indent) const override
    {
        PropertyNode::print(cout, indent);
        cout << "size: " << _size;
    }
};

class ColorNode : public PropertyNode
{
    ColorType _color;

public:

    ColorNode(ColorType color)
    :
        PropertyNode{PropertyType::Color},
        _color{color} {}

    ColorType color() const { return _color; }

    void print(std::ostream& cout, int indent) const override
    {
        PropertyNode::print(cout, indent);
        cout << _color;
    }
};

class ObjectNode
{
    ObjectType _object_type;

public:
    // to get children in associative way
    std::map<PropertyType, const PropertyNode*> props;

    // to get subobjects in assciative way
    // object can have several subobjects of the same type
    std::multimap<ObjectType, const ObjectNode*> subobjects;

    ObjectNode(ObjectType object_type) : _object_type{object_type} {}

    ObjectType type() const { return _object_type; }

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


class SketchNode : public ParseNode
{
    // just need to store bunch of nodes
    std::vector<ObjectNode*> children;

public:

    SketchNode() : ParseNode{ParseType::Senteniel} {}

    void addChild(ObjectNode* child) { children.push_back(child); }

    children_iter childrenBegin() const { return children.cbegin(); }
    children_iter childrenEnd()   const { return children.end(); }

    virtual void print(std::ostream& cout, int indent) const
    {
        ParseNode::print(cout, indent);

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

   SketchNode* getSketch()
   {
        SketchNode* Sketch = new SketchNode; // senteniel node

        for (ObjectNode* new_object = getObj(); new_object; new_object = getObj())
        {
            std::cout << "MEOW\n";
            Sketch->addChild(new_object);
        }

        return Sketch;
   }
    
    #define REQUIRE(token_type) if (!require(token_type))                                        \
                                {   std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << '\n'; \
                                    return nullptr;}
    
   ObjectNode* getObj()
   {
        std::cout << "getObj()\n";

        REQUIRE(TokenType::ObjectType);

            // TODO more clear way to code it
            const QualifyToken* token = dynamic_cast<const QualifyToken*>(grab());
            ObjectType obj_type = static_cast<ObjectType>(token->specific());
            ObjectNode* obj_node = new ObjectNode(obj_type); 

        PropertyNode* size_node = getSize();
            obj_node->props[size_node->type()] = size_node;

        if (PropertyNode* position_node = getPos(); position_node)
            obj_node->props[position_node->type()] = position_node;

        REQUIRE(TokenType::LCurl); grab();
        
        while(true)
        {
            std::cout << "[info] dig out arguments\n";
            if (PropertyNode* prop_node = getProp(); prop_node)
            {
                obj_node->props[prop_node->type()] = prop_node;
            }
            else if (const ObjectNode* new_object = getObj(); new_object)
            {
                obj_node->subobjects.insert({new_object->type(), new_object});
            }
            else
            {
                break;
            }
        }

        REQUIRE(TokenType::RCurl); grab();

        return obj_node;
   }

   PropertyNode* getSize()
   {
    std::cout << "getSize()\n";

    int size = 0;

    for (; require(TokenType::SizeScale); grab())
        ++size;

    PropertyNode* size_node = new SizeNode{size};

    return size_node;
   }

    PropertyNode* getPos()
    {
        std::cout << "getPos()\n";
        // [get a position] e.g. "[10,10]"

        REQUIRE(TokenType::LBrace); grab(); // '['

        REQUIRE(TokenType::Number); // '10'
            auto x = dynamic_cast<const QualifyToken&>(*grab());

        REQUIRE(TokenType::Comma); grab(); // ','

        REQUIRE(TokenType::Number); // '10'
            auto y =  dynamic_cast<const QualifyToken&>(*grab());

        REQUIRE(TokenType::RBrace); grab(); // ']'

        PropertyNode* position_node = new PositionNode{x.specific(), y.specific()};

        return position_node;
    }

    PropertyNode* getProp()
    {
        std::cout << "getProp()\n";

        REQUIRE(TokenType::Property);

        auto token = dynamic_cast<const QualifyToken*>(grab());
        auto prop_type = static_cast<PropertyType>(token->specific());

        REQUIRE(TokenType::Assign); grab();

        PropertyNode* prop_node = nullptr;

        switch(prop_type)
        {
            case PropertyType::Color:
            {
                REQUIRE(TokenType::Color);
                auto token = dynamic_cast<const QualifyToken*>(grab());
                auto color = static_cast<ColorType>(token->specific());
                prop_node = new ColorNode{color};
                break;
            }
            case PropertyType::Size:
            {
                REQUIRE(TokenType::Number);
                auto token = dynamic_cast<const QualifyToken*>(grab());
                auto size = static_cast<int>(token->specific());
                prop_node = new SizeNode{size};
            }
            default:

                std::cout << "[error] Unrecognised property type\n";
                REQUIRE(TokenType::Unknown); grab();
                prop_node = nullptr;
        }

        REQUIRE(TokenType::SemiColon); grab();

        return prop_node;
    }

    #undef REQUIRE
};

