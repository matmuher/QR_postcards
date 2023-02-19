#pragma once

#include <set>
#include "Tokens.hpp" // pixel_color
#include "magic_enum.hpp"

class Object
{
    const int DEFAULT_INT = -1;

    ObjectType _type = ObjectType::Unknown;
    ColorType _color = ColorType::Unknown;
    int _size   = DEFAULT_INT;
    int _x      = DEFAULT_INT;
    int _y      = DEFAULT_INT;

public:

    inline static const std::set<PropertyType> props = {
                                    PropertyType::Position,
                                    PropertyType::Size,
                                    PropertyType::Color
                                };

    Object(ObjectType type, ColorType color, int size, int x, int y)
    :
        _type{type},
        _color{color},
        _size{size},
        _x{x},
        _y{y} {}

    Object(ObjectType type) : _type{type} {}
    Object() {};

    void set_type(ObjectType type) { _type = type; }

    void set_color(ColorType color) { _color = color; }
    
    void set_size(int size) { _size = size; }
    
    void set_x(int x) { _x = x; }
    
    void set_y(int y) { _y = y; }

    ObjectType type() const { return _type; }
    ColorType color() const { return _color; }
    int size() const { return _size; }
    int x() const { return _x; }
    int y() const { return _y; }

    virtual void print(std::ostream& cout) const
    {
        cout << _type << '\n';
        
        cout << "\tColor: " << _color << '\n'
             << "\tPosition: [" << _x << ',' << _y << "]\n"
             << "\tSize: " << _size;
    }
};
