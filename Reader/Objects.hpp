#pragma once

#include <set>
#include "Tokens.hpp" // pixel_color

struct Point
{
    int x, y;
};

class Object
{
    // standard properties
    std::set<PropertyType> props = {
                                        PropertyType::Position,
                                        PropertyType::Size,
                                        PropertyType::Color
                                    };

    ObjectType _type;
    ColorType _color;
    int _size;
    int _x;
    int _y;

public:

    Object(ObjectType type, ColorType color, int size, int x, int y)
    :
        _type{type},
        _color{color},
        _size{size},
        _x{x},
        _y{y} {}

    ObjectType type() const { return _type; }
    ColorType color() const { return _color; }
    int size() const { return _size; }
    int x() const { return _x; }
    int y() const { return _y; }
};
