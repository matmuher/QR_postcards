#pragma once

#include <Parser.hpp>
#include <EnumPrinter.hpp>

class Creator
{
    const SketchNode* _root = nullptr;
    std::vector<Object*> objects;

    template<class T>
    const T* downcast_prop(const PropertyNode* prop_node)
    {
        return dynamic_cast<const T*>(prop_node);
    } 

public:

    Creator(const SketchNode* root) : _root{root} {};

    Creator() {}

    void initialize(SketchNode* root)
    {
        _root = root;
        objects.clear();
    }

    Object* spawn_default(ObjectType obj_type)
    {
        switch(obj_type)
        {
            case ObjectType::Pine:
                return new Pine;

            case ObjectType::Star:
                return new Star;

            case ObjectType::Gift:
                return new Gift;

            default:
                return nullptr;
        }

        return nullptr;
    }

    Object* create_default(const ObjectNode* obj_node, ObjectType obj_type)
    {
        Object* obj = spawn_default(obj_type);

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
                        obj->set_size(downcast_prop<NumberNode>(prop_node)->num());
                        break;

                    case PropertyType::Intensity:
                        obj->set_action_intensity(downcast_prop<NumberNode>(prop_node)->num());

                    default:

                        std::cout << "[error] " << str_enum(prop) << "is not processed yet\n";
                        break;
                }
            }
            else
            {
                std::cout << "[error] " << str_enum(prop) << "is not specified for " 
                          << str_enum(obj_type) << '\n';
            }
        }
    

        return obj;
    }

    /*
        I wanna:
            1. get vector of lines for this congrat
            2. iterate it and according to it construct N, subobjects for
            3. push to Congrats vector
    */
    Object* create_congratulation(ObjectNode* obj_node)
    {
        auto congrat_node = dynamic_cast<CongratNode*>(obj_node);
        Congratulation* congrat = new Congratulation();

        for (const auto& line_node : congrat_node->line_nodes)
        {
            congrat->add_line(line_node->msg());
        }

        return congrat;
    }

    Object* create_obj(ObjectNode* obj_node)
    {
        ObjectType obj_type = obj_node->type();

        Object* obj = nullptr;
        switch(obj_type)
        {
            // [default obejects]
            case ObjectType::Pine:
            case ObjectType::Star:
            case ObjectType::Gift:

                obj = create_default(obj_node, obj_type);
                break;
            
            case ObjectType::Congratulation:

                obj = create_congratulation(obj_node);
                break;

            //case ObjectType::Text:
            default:
                break;
        }

        return obj;
    }

    const std::vector<Object*> create() // TODO return rvalue?
    {
        auto obj_end = _root->childrenEnd();

        for (auto obj_it  = _root->childrenBegin(); obj_it != obj_end; ++obj_it)
        {
            Object* obj = create_obj(dynamic_cast<ObjectNode*>(*obj_it));
            objects.push_back(obj);
        }

        return objects;
    }
};