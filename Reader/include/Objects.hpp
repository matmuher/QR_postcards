#pragma once

#include <set>
#include <magic_enum.hpp>
#include <Tokens.hpp>
#include <memory>
#include <EnumPrinter.hpp>
#include <vector>


enum class ObjectType
{
    Pine,
    Star,
    Gift,
    Congratulation,
    Line,
    PineTop,
    Unknown
};


enum class PropertyType
{
    Size,
    Color,
    Position,
    Text,
    Unknown
};


enum class ColorType
{
    White,
    Red,
    Violet,
    Blue,
    Yellow,
    Unknown
};


//-----------------------------------------------------------CLASS_BASE_OBJECT-----------------------------------------------------------------


class BaseObject
{

protected:

    ObjectType _type = ObjectType::Unknown;
    ColorType _color = ColorType::Unknown;
    int _action_intensity = 0;

public:

    BaseObject(ObjectType type, ColorType color, int action_intensity)
    :
        _type{type},
        _color{color},
        _action_intensity{action_intensity} {}

    BaseObject(ObjectType type) : _type{type} {}
    BaseObject(ObjectType type, ColorType color) : _type{type}, _color{color} {}

    BaseObject() {}

    void set_type(ObjectType type) { _type = type; }
    void set_color(ColorType color) { _color = color; }
    void set_action_intensity(int action_intensity) { _action_intensity = action_intensity; }

    ObjectType type() const { return _type; }
    ColorType color() const { return _color; }
    int action_intensity() const { return _action_intensity; }
};


//-----------------------------------------------------------CLASS_OBJECT-----------------------------------------------------------------


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

    Object(int x, int y, ColorType color, ObjectType type) : BaseObject{type, color}, _x(x), _y(y) {};
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
        cout << str_enum(_type) << '\n';
        
        cout << "\tColor: " << str_enum(_color) << '\n'
             << "\tPosition: [" << _x << ',' << _y << "]\n"
             << "\tSize: " << _size << '\n'
             << "\tAction intensity: " << _action_intensity << '\n';
    }
};


//-----------------------------------------------------------CLASS_STAR-----------------------------------------------------------------


class Star : public Object
{
private:

    int _light_power = 0;

public:

    Star() : Object{ObjectType::Star} {}
    Star(int x, int y, ColorType color) : Object(x, y, color, ObjectType::Star) {};
    Star(Object &obj) : Object(obj) {};
    Star(const Star& star) : Object(star.x(), star.y(), star.color(), ObjectType::Star) {};


    void set_light_power(int light_power) { _light_power = light_power; }
    int light_power() const { return _light_power; }
};


//-----------------------------------------------------------CLASS_PINE------------------------------------------------------------------


class PineTop;
class Pine : public Object
{
private:

    std::unique_ptr<PineTop> _pine_top{};

public:

    Pine() : Object{ObjectType::Pine} {}
    Pine(int x, int y, ColorType color) : Object(x, y, color, ObjectType::Pine) {};
    Pine(Object &obj) : Object(obj) {};
    Pine(const Pine& pine) : Object(pine.x(), pine.y(), pine.color(), ObjectType::Pine) {};

    void set_pine_top(std::unique_ptr<PineTop> pine_top) { _pine_top = std::move(pine_top); }
    const PineTop& pine_top() const { return *_pine_top; }
};


//-----------------------------------------------------------CLASS_GIFT------------------------------------------------------------------


class Gift : public Object
{

public:

    Gift() : Object{ObjectType::Gift} {}
    Gift(int x, int y, ColorType color) : Object(x, y, color, ObjectType::Gift) {};
    Gift(Object &obj) : Object(obj) {};
    Gift(const Pine& gift) : Object(gift.x(), gift.y(), gift.color(), ObjectType::Gift) {};

};


//-----------------------------------------------------------CLASS_PINE_TOP-----------------------------------------------------------------


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


//-----------------------------------------------------------CLASS_LINE-----------------------------------------------------------------


class Line : public BaseObject
{
    std::string _msg;

public:

    Line(const std::string& msg)
    :
        BaseObject{ObjectType::Line},
        _msg{msg} {}
    
    const std::string& get_msg() const
    {
        return _msg;
    }
};


//-----------------------------------------------------------CLASS_CONGRATULATION-----------------------------------------------------------------


class Congratulation : public Object
{
    /*
    1)
        in most cases it's better to use vector of objects,
        not pointers to objects. as it is more cache-friendly.
        cool benchmark on this topic:
            https://www.cppstories.com/2014/05/vector-of-objects-vs-vector-of-pointers/
    */
    std::vector<Line> lines;

public:

    Congratulation() : Object{ObjectType::Congratulation} {};
    Congratulation(int x, int y, ColorType color) : Object(x, y, color, ObjectType::Congratulation) {};

    void add_line(const Line& line)
    {
        lines.push_back(line);
    }

    const std::vector<Line>& get_lines() const
    {
        return lines;
    }

    virtual void print(std::ostream& cout) const
    {
        Object::print(cout);
        for (auto& line : lines)
            cout << "\t\t" << line.get_msg() << '\n';
    }
};
