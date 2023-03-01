


enum Object_type
{
	PINE,
	STAR,
};

enum Color
{
	WHITE,
	RED,
	VIOLET,
	BLUE,
};

class Object
{

protected:

	int x_, y_;
	Object_type type_;
	Color color_;

public:

	Object(int x, int y, Object_type type, Color color) : x_(x), y_(y), type_(type), color_(color) {};
	Object() : Object(0, 0, Object_type::PINE, Color::BLUE) {};
	Object(Object &obj) : Object(obj.x(), obj.y(), obj.type(), obj.color()) {};
	virtual ~Object() = default;

	virtual int x() const { return x_; }
	virtual int y() const { return y_; }
	virtual Object_type type() const { return type_; }
	virtual Color color() const { return color_; }

};

class Pine : virtual public Object
{
public:
	Pine(int x, int y, Color color = Color::BLUE) : Object(x, y, Object_type::PINE, color) {};
	Pine(Object &obj) : Object(obj) {};
	Pine(const Pine& pine) : Object(pine.x(), pine.y(), Object_type::PINE, Color::BLUE) {};
};

class Star : virtual public Object
{
public:
	Star(int x, int y, Color color = Color::WHITE) : Object(x, y, Object_type::STAR, color) {};
	Star(Object &obj) : Object(obj) {};
	Star(const Star& star) : Object(star.x(), star.y(), Object_type::STAR, Color::WHITE) {};
};
