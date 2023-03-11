#pragma once

#include <Parser.hpp>

class Creator
{

    const SketchNode* _root = nullptr; // redundant? is set by default?
    std::vector<const Object*> objects;

    template<class T>
    const T* downcast_prop(const PropertyNode* prop_node)
    {
        return dynamic_cast<const T*>(prop_node);
    } 

public:

    Creator(const SketchNode* root) : _root{root} {};

    const Object* create_default(const ObjectNode* obj_node, ObjectType obj_type)
    {
        Object* obj = new Object(obj_type);

        for (PropertyType prop : Object::props)
        {
            if (obj_node->props.find(prop) != obj_node->props.end())
            {
                const PropertyNode* prop_node = obj_node->props.at(prop);

                switch (prop)
                {
                    case PropertyType::Color:
                        obj->set_color(downcast_prop<ColorNode>(prop_node)->color());
                        break;

                    case PropertyType::Position:
                        obj->set_x(downcast_prop<PositionNode>(prop_node)->x());
                        obj->set_y(downcast_prop<PositionNode>(prop_node)->y());
                        break;

                    case PropertyType::Size:
                        obj->set_size(downcast_prop<SizeNode>(prop_node)->size());
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
        ObjectType obj_type = obj_node->type();

        const Object* obj = nullptr;
        switch(obj_type)
        {
            // [default obejects]
            case ObjectType::Pine:
            case ObjectType::Star:
            case ObjectType::Gift:

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