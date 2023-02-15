#pragma once

#include "Tokens.hpp" // pixel_color

struct Point
{
    int x, y;
};

class Object
{
    ObjectType _type;
    pixel_color _color;
    int _size;
    Point _pos;

public:

    Object(ObjectType type, pixel_color color, int size, Point pos)
    :
        _type{type},
        _color{color},
        _size{size},
        _pos{pos} {}

    ObjectType type() const { return _type; }
    pixel_color color() const { return _color; }
    int size() const { return _size; }
    Point pos() const { return _pos; }
};
