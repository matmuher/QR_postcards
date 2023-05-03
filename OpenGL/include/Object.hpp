#ifndef OBJECT_H
#define OBJECT_H


/// Типы объектов
enum Object_type
{
	PINE,
	STAR,
	GIFT,
};


/// Цвета объектов
enum Color
{
	WHITE,
	RED,
	VIOLET,
	BLUE,
	YELLOW,
};


//--------------------------------------------------Object--------------------------------------------------------


/**
 * @brief class Object явлектся базовым для контретных объектов, которые могут располагаться на экране
 */
class Object
{

protected:

	/// Координаты объектов
	float x_, y_;
	Object_type type_;
	Color color_;

public:

	Object(float x, float y, Object_type type, Color color) : x_(x), y_(y), type_(type), color_(color) {};
	Object() : Object(0, 0, Object_type::PINE, Color::BLUE) {};
	Object(Object &obj) : Object(obj.x(), obj.y(), obj.type(), obj.color()) {};
	virtual ~Object() = default;

	virtual float x() const { return x_; }
	virtual float y() const { return y_; }
	virtual Object_type type() const { return type_; }
	virtual Color color() const { return color_; }

};


//--------------------------------------------------Pine-------------------------------------------------------


class Pine : virtual public Object
{
public:
	Pine(float x, float y, Color color = Color::BLUE) : Object(x, y, Object_type::PINE, color) {};
	Pine(Object &obj) : Object(obj) {};
	Pine(const Pine& pine) : Object(pine.x(), pine.y(), Object_type::PINE, Color::BLUE) {};
};


//--------------------------------------------------Star--------------------------------------------------------


class Star : virtual public Object
{
public:
	Star(float x, float y, Color color = Color::WHITE) : Object(x, y, Object_type::STAR, color) {};
	Star(Object &obj) : Object(obj) {};
	Star(const Star& star) : Object(star.x(), star.y(), Object_type::STAR, Color::WHITE) {};
};


//--------------------------------------------------Gift--------------------------------------------------------


class Gift : virtual public Object
{
public:
	Gift(float x, float y, Color color = Color::BLUE) : Object(x, y, Object_type::GIFT, color) {};
	Gift(Object &obj) : Object(obj) {};
	Gift(const Gift& gift) : Object(gift.x(), gift.y(), Object_type::GIFT, Color::BLUE) {};
};


#endif
