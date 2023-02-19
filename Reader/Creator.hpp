#pragma once

#include "Parser.hpp"

class Creator
{

    const ParseNode* _root = nullptr; // redundant? is set by default?
    std::vector<const Object*> objects;

public:

    Creator(const ParseNode* root) : _root{root} {};

    int get_num(const ParseNode* prop_node, int child_index)
    {
        if (prop_node->childrenBegin() != prop_node->childrenEnd())
        {
            auto child_it = prop_node->childrenBegin();

            while (child_it != prop_node->childrenEnd() && child_index > 0)
            {
                --child_index;
                ++child_it;
            }

            if (child_it != prop_node->childrenEnd())
            {    
                auto child_node = static_cast<const NumberNode*>(*(child_it));
                return child_node->num();
            }

            return -1;
        }
        
        std::cout << "[error] this property node has no children\n";
        return -1;
    }

    int get_num(const ParseNode* prop_node)
    {
        return get_num(prop_node, 0);
    }

    const Object* create_default(const ObjectNode* obj_node, ObjectType obj_type)
    {
        Object* obj = new Object(obj_type);

        for (PropertyType prop : Object::props)
        {
            if (obj_node->props.find(prop) != obj_node->props.end())
            {
                const ParseNode* prop_node = obj_node->props.at(prop);

                switch (prop)
                {
                    case PropertyType::Color:
                        obj->set_color(static_cast<ColorType>(get_num(prop_node)));
                        break;

                    case PropertyType::Position:
                        obj->set_x(get_num(prop_node, 0));
                        obj->set_y(get_num(prop_node, 1));
                        break;

                    case PropertyType::Size:
                        obj->set_size(get_num(prop_node));
                        break;

                    default:
                        std::cout << "[error] " << prop << " is not processed yet\n";
                        break;
                }
            }
            else
            {
                std::cout << "[error] no " << prop << " is specified for" << obj_type << '\n';
                break;
            }
        }

        return obj;
    }

    const Object* create_obj(const ObjectNode* obj_node)
    {
        ObjectType obj_type = obj_node->object_type();

        const Object* obj = nullptr;
        switch(obj_type)
        {
            // [default obejects]
            case ObjectType::Pine:
            case ObjectType::Star:

                obj = create_default(obj_node, obj_type);
                break;
            
            //case ObjectType::Text:
            default:
                break;
        }

        return obj;
    }

    const std::vector<const Object*>& create() // TODO return rvalue?
    {
        auto obj_end = _root->childrenEnd();

        for (auto obj_it  = _root->childrenBegin(); obj_it != obj_end; ++obj_it)
        {
            const Object* obj = create_obj(dynamic_cast<const ObjectNode*>(*obj_it));
            objects.push_back(obj);
        }

        return objects;
    }
};