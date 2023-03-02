#pragma once

#include <set>
#include <magic_enum.hpp>
#include <Tokens.hpp> // pixel_color
#include <memory>

class BaseObject
{
protected:

    int _action_intensity = 0;
    ObjectType _type = ObjectType::Unknown;
    ColorType _color = ColorType::Unknown;

public:

    BaseObject(ObjectType type, ColorType color, int action_intensity)
    :
        _type{type},
        _color{color},
        _action_intensity{action_intensity} {}

    BaseObject(ObjectType type) : _type{type} {}

    BaseObject() {}

    void set_type(ObjectType type) { _type = type; }
    void set_color(ColorType color) { _color = color; }
    void set_action_intensity(int action_intensity) { _action_intensity = action_intensity; }

    ObjectType type() const { return _type; }
    ColorType color() const { return _color; }
    int action_intensity() const { return _action_intensity; }
};

// PinnedObject
class Object : public BaseObject
{
private:

    int _size = 0;
    int _x = 0;
    int _y = 0;

public:

    inline static const std::set<PropertyType> props = {
                                    PropertyType::Position,
                                    PropertyType::Size,
                                    PropertyType::Color
                                };

    Object(ObjectType type) : BaseObject{type} {}
    Object() {};
    
    void set_size(int size) { _size = size; }
    void set_x(int x) { _x = x; }
    void set_y(int y) { _y = y; }

    int size() const { return _size; }
    int x() const { return _x; }
    int y() const { return _y; }

    virtual void print(std::ostream& cout) const
    {
        cout << _type << '\n';
        
        cout << "\tColor: " << _color << '\n'
             << "\tPosition: [" << _x << ',' << _y << "]\n"
             << "\tSize: " << _size << '\n'
             << "\tAction intensity: " << _action_intensity << '\n';
    }
};

class Star : public Object
{
private:

    int _light_power = 0;

public:

    Star() : Object{ObjectType::Star} {}

    void set_light_power(int light_power) { _light_power = light_power; }
    int light_power() const { return _light_power; }
};

class PineTop;
class Pine : public Object
{
private:

    std::unique_ptr<PineTop> _pine_top{};

public:

    Pine() : Object{ObjectType::Pine} {}

    void set_pine_top(std::unique_ptr<PineTop> pine_top) { _pine_top = std::move(pine_top); }
    const PineTop& pine_top() const { return *_pine_top; }
};

class PineTop : public BaseObject
{
public:

    enum class TopType
    {
        Star,
        Snowflake,
        Icile,
        Empty
    };

private:

    TopType _type;

public:

    PineTop()
    :
        BaseObject{ObjectType::PineTop},
        _type{TopType::Empty} {}

    void set_type(TopType type) { _type = type; }
    TopType type() const { return _type; }
};
