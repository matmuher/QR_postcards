#pragma once

#include <map>
#include <vector>
#include <magic_enum.hpp>

#include <Tokenizer.hpp>
#include <Objects.hpp>
#include <EnumPrinter.hpp>
#include <TextProcessExceptions.hpp>

class ParseNode;
class PropertyNode;
class ObjectNode;

#ifdef NDEBUG

    #define LOG(arg)

#else

    #define LOG(arg) std::cout << arg

#endif

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

void print_indent(std::ostream& cout, int indent)
{
    while (indent--) LOG( ' ');
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
        LOG( str_enum(_type));
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
        LOG("x: " << _x << ", y: " << _y);
    }
};

class NumberNode : public PropertyNode
{
    int _num = -1;

public:

    NumberNode(int num, PropertyType type)
    :
        PropertyNode{type},
        _num{num} {}

    int num() const { return _num; }

    void print(std::ostream& cout, int indent) const override
    {
        PropertyNode::print(cout, indent);
        LOG("num: " << _num);
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
        print_enum(cout, _color);
    }
};

class ObjectNode
{
    ObjectType _object_type;

public:
    // to get properties in associative way
    std::map<PropertyType, const PropertyNode*> props;

    ObjectNode(ObjectType object_type) : _object_type{object_type} {}

    ObjectType type() const { return _object_type; }

    virtual void print(std::ostream& cout, int indent) const
    {
        print_indent(cout, indent);
        print_enum(cout, _object_type) << '\n';

        for (auto& elem : props)
        {
            print_indent(cout, indent+4);
            print_enum(cout, elem.first) << ":\n";
            elem.second->print(cout, indent+8);
            LOG('\n');
        }
    }
};

// [HAS PROPERTIES]
class LineNode : public ObjectNode
{
    std::string _msg;

public:

    LineNode(const std::string& msg)
    :
        ObjectNode{ObjectType::Line},
        _msg{msg} {}

    std::string msg() const { return _msg; }
};

// To be honest - better to create abstract class for AbsObj,
// as CongratNode do not have any properties
// [HAS NO PROPERTIES]
class CongratNode : public ObjectNode
{
public:

    std::vector<LineNode*> line_nodes;

    CongratNode() : ObjectNode{ObjectType::Congratulation} {};

    virtual void print(std::ostream& cout, int indent) const
    {
        ObjectNode::print(cout, indent);

        for (auto& line_node : line_nodes)
        {
            print_indent(cout, indent+4);
            LOG( "Line:\n");
            print_indent(cout, indent+8);
            LOG(  line_node->msg() << "\n");
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
            LOG( '\n');
            (*st)->print(cout, indent+4);
            LOG( ' ');
            ++st;
        }
    }
};

struct ParseError
{
    Token& user_token;
    TokenType wanted_token;
};

class Parser
{
    std::vector<Object> objects;

    const std::deque<Token*>* _token_que;

    std::deque<Token*>::const_iterator walker;
    std::deque<Token*>::const_iterator end;

    int _background_id = 0;

public:

    Parser(const std::deque<Token*>& token_que)
    :
        _token_que{&token_que},
        walker{_token_que->begin()},
        end{_token_que->end()} {}

    Parser() {}

    int get_background() const { return _background_id; }

    void initialize(const std::deque<Token*>& token_que)
    {
        _token_que = &token_que;
        walker = _token_que->begin();
        end = _token_que->end();
        _background_id = 0;

        objects.clear();
    }

    // TODO syntax errors handling
    bool require_token(TokenType required_token, bool should_throw = true)
    {
        if (walker == end)
        {
            LOG( "[info] end of token list\n");
            return false;
        }

        Token* cur_token = *walker;
        if (cur_token->type() != required_token)
        {
            LOG( "[warn] wanted " << str_enum(required_token) << '\n'
                      << "       got " << *(*walker) << "\n\n");

            if (should_throw)
                throw parse_error{required_token, cur_token->type(),
                                  cur_token->line_id(), cur_token->start()};
            
            return false;
        }
        else
            LOG( "[info] got" << str_enum(required_token) << "\n\n");

        return true;
    }

    bool try_token(TokenType required_token)
    {
        return require_token(required_token, false);
    }

    const Token* grab()
        {   return *walker++; }

   SketchNode* getSketch()
   {
        if (try_token(TokenType::Background))
        {
            grab();
            require_token(TokenType::LBrace); grab();
            
            require_token(TokenType::Number);
            auto background_id_token = dynamic_cast<const QualifyToken&>(*grab());
            _background_id = background_id_token.specific();

            require_token(TokenType::RBrace); grab();
            require_token(TokenType::SemiColon); grab();
        }

        SketchNode* Sketch = new SketchNode; // senteniel node

        for (ObjectNode* new_object = getObj(); new_object; new_object = getObj())
        {
            LOG( "MEOW\n");
            Sketch->addChild(new_object);
        }

        return Sketch;
   }

   ObjectNode* getObj()
   {
        LOG( "getObj()\n");

        if (!try_token(TokenType::ObjectType))
        {
            return nullptr;
        }
            // TODO more clear way to code it
            const QualifyToken* token = dynamic_cast<const QualifyToken*>(grab());
            ObjectType obj_type = static_cast<ObjectType>(token->specific());

            ObjectNode* obj_node = obj_type == ObjectType::Congratulation ? new CongratNode() : new ObjectNode(obj_type); 

        PropertyNode* size_node = getSize();
            obj_node->props[size_node->type()] = size_node;

        if (PropertyNode* position_node = getPos(); position_node)
            obj_node->props[position_node->type()] = position_node;

        require_token(TokenType::LCurl); grab();
        
        while(true)
        {
            LOG( "[info] dig out arguments\n");
            if (LineNode* line_node = getLine();
                obj_type == ObjectType::Congratulation && line_node)
            {
                dynamic_cast<CongratNode*>(obj_node)->line_nodes.push_back(line_node);
            }
            else if (PropertyNode* prop_node = getProp(); prop_node)
            {
                obj_node->props[prop_node->type()] = prop_node;
            }
            else
            {
                break;
            }
        }

        require_token(TokenType::RCurl); grab();

        return obj_node;
   }

   PropertyNode* getSize()
   {
    LOG( "getSize()\n");

    int size = 1;

    for (; try_token(TokenType::SizeScale); grab())
        ++size;

    PropertyNode* size_node = new NumberNode{size, PropertyType::Size};

    return size_node;
   }

    PropertyNode* getPos()
    {
        LOG( "getPos()\n");
        // [get a position] e.g. "[10,10]"

        if (!try_token(TokenType::LBrace))
        {
            return nullptr;
        }

        grab(); // '['

        require_token(TokenType::Number); // '10'
            auto x = dynamic_cast<const QualifyToken&>(*grab());

        require_token(TokenType::Comma); grab(); // ','

        require_token(TokenType::Number); // '10'
            auto y =  dynamic_cast<const QualifyToken&>(*grab());

        require_token(TokenType::RBrace); grab(); // ']'

        PropertyNode* position_node = new PositionNode{x.specific(), y.specific()};

        return position_node;
    }

    PropertyNode* getProp()
    {
        LOG( "getProp()\n");

        if (!try_token(TokenType::Property))
        {
            return nullptr;
        }

        auto token = dynamic_cast<const QualifyToken*>(grab());
        auto prop_type = static_cast<PropertyType>(token->specific());

        require_token(TokenType::Assign); grab();

        PropertyNode* prop_node = nullptr;

        switch(prop_type)
        {
            case PropertyType::Color:
            {
                require_token(TokenType::Color);
                auto token = dynamic_cast<const QualifyToken*>(grab());
                auto color = static_cast<ColorType>(token->specific());
                prop_node = new ColorNode{color};
                break;
            }
            case PropertyType::Size:
            {
                require_token(TokenType::Number);
                auto token = dynamic_cast<const QualifyToken*>(grab());
                auto size = static_cast<int>(token->specific());
                prop_node = new NumberNode{size, PropertyType::Size};
                break;
            }
            case PropertyType::Intensity:
            {
                require_token(TokenType::Number);
                auto token = dynamic_cast<const QualifyToken*>(grab());
                auto num = static_cast<int>(token->specific());
                prop_node = new NumberNode{num, PropertyType::Intensity};    
                break;
            }
            default:

                LOG( "[warn] Unrecognised property type\n");
                prop_node = nullptr;
                break;
        }

        require_token(TokenType::SemiColon); grab();

        return prop_node;
    }

    // here new line object node should be constructed
    LineNode* getLine()
    {
        if (!try_token(TokenType::Plus))
        {
            return nullptr;
        }
        
        grab(); // '+'

            require_token(TokenType::String);
    
        auto token = grab();
    
        LineNode* line_node = new LineNode(std::string(token->start(), token->end()));

        if(try_token(TokenType::LCurl))
        {
            grab();
                
            while(true)
            {
                LOG( "[info] dig out arguments\n");

                if (PropertyNode* prop_node = getProp(); prop_node)
                {
                    line_node->props[prop_node->type()] = prop_node;
                }
                else
                {
                    break;
                }
            }

            require_token(TokenType::RCurl); grab();
        }
        else
        {
            require_token(TokenType::SemiColon); grab();
        }

        return line_node;
    }
};

