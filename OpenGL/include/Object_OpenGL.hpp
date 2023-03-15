#include "Objects.hpp"

#include "Model.hpp"
#include <vector>


int create_VAO_and_shader_for_cube(unsigned int *cubeVAO, Shader *program);
std::vector<Star*> Lights;


//--------------------------------------------------ABSTRACT_BASE_CLASS_OBJECT_OpenGL----------------------------------------------------------


class Object_OpenGL
{

protected:

    static Shader program_;
    glm::mat4 model_;   
    Model Model_obj_;

public:

	//Object_OpenGL(Object &object);
    Object_OpenGL() { }
	virtual ~Object_OpenGL() = default;

	virtual void draw() = 0;
    static int create_program(const glm::mat4 &view, const glm::mat4 &projection);
    static Shader &program() { return program_; }

};

Shader Object_OpenGL::program_;

//-----------------------------------------------------------CLASS_PINE_OpenGL-----------------------------------------------------------------


class Pine_OpenGL : public Object_OpenGL
{
    Pine pine_;

public:

	Pine_OpenGL(Object *pine, const glm::mat4 &view, const glm::mat4 &projection);
	void draw() override;
    Pine pine() const { return pine_; }

};


//-----------------------------------------------------------CLASS_GIFT_OpenGL-----------------------------------------------------------------


class Gift_OpenGL : public Object_OpenGL
{
    Gift gift_;

public:

    Gift_OpenGL(Object *gift, const glm::mat4 &view, const glm::mat4 &projection);
    void draw() override;
    Gift gift() const { return gift_; }

};


//-----------------------------------------------------------CLASS_STAR_OpenGL-----------------------------------------------------------------



class Star_OpenGL : public Object_OpenGL
{
	static Shader program_;
    glm::vec3 color_;
    Star star_;

public:

	Star_OpenGL(Object *star, const glm::mat4 &view, const glm::mat4 &projection);
    void draw() override;
    int create_program(const glm::mat4 &view, const glm::mat4 &projection);
    Star star() const { return star_; }

};

Shader Star_OpenGL::program_;


//------------------------------------------------------------Objects_methods------------------------------------------------------------------


//-------------------------------------------------------------CONSTRUCTORS--------------------------------------------------------------------


Pine_OpenGL::Pine_OpenGL(Object *pine, const glm::mat4 &view, const glm::mat4 &projection) : pine_(*pine)
{
    Model_obj_ = Model((std::filesystem::path("../objects/Christmas_tree/Tree.obj")).c_str());

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, glm::vec3(pine_.x(), pine_.y(), 0.0f));
    
    std::cout << "pine coords: " << pine_.x() << "  " << pine_.y() << std::endl;

}


Gift_OpenGL::Gift_OpenGL(Object *gift, const glm::mat4 &view, const glm::mat4 &projection) : gift_(*gift)
{
    Model_obj_ = Model((std::filesystem::path("../objects/gift/gift.obj")).c_str());

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, glm::vec3(gift_.x(), gift_.y(), 0.0f));
    model_ = glm::scale(model_, glm::vec3(0.04f));
    model_ = glm::rotate(model_, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    std::cout << "gift coords: " << gift_.x() << "  " << gift_.y() << std::endl;

}


Star_OpenGL::Star_OpenGL(Object *star, const glm::mat4 &view, const glm::mat4 &projection) : star_(*star)
{
	static int init_program = create_program(view, projection);

    Model_obj_ = Model((std::filesystem::path("../objects/star/star.obj")).c_str());

    program_.use_Program();
  
    switch (star_.color())
    {
        case ColorType::White:
        {
            color_ = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        }
        case ColorType::Violet:
        {
            color_ = glm::vec3(1.0f, 0.0f, 1.0f);
            break;
        }
        case ColorType::Blue:
        {
            color_ = glm::vec3(0.0f, 1.0f, 1.0f);
            break;
        }
        case ColorType::Red:
        {
            color_ = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }
        case ColorType::Yellow:
        {
            color_ = glm::vec3(1.0f, 1.0f, 0.0f);
            break;
        }
    }
    
    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, glm::vec3(star_.x(), star_.y(), 3.0f));
    model_ = glm::scale(model_, glm::vec3(0.1f));
    model_ = glm::rotate(model_, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    std::cout << "star coords: " << star_.x() << " " << star_.y() << std::endl;
}


//-----------------------------------------------------------------Draw()-------------------------------------------------------------------


void Star_OpenGL::draw()
{
    program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    
    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));

    program_.set_matrix4fv("model", model_);
    program_.set_matrix3fv("norm_model", norm_model);
    program_.set_vec3f("light_color", color_);
 
    Model_obj_.draw(program_);

}


void Pine_OpenGL::draw()
{
	Object_OpenGL::program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));

    Object_OpenGL::program_.set_matrix4fv("model", model_);
    Object_OpenGL::program_.set_matrix3fv("norm_model", norm_model);
    Object_OpenGL::program_.set_float("ambientStrength", 0.2f);

    Model_obj_.draw(Object_OpenGL::program_);
}


void Gift_OpenGL::draw()
{
    Object_OpenGL::program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 1.0f));

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));
    
    Object_OpenGL::program_.set_matrix4fv("model", model_);
    Object_OpenGL::program_.set_matrix3fv("norm_model", norm_model);
    Object_OpenGL::program_.set_float("ambientStrength", 1.0f);

    Model_obj_.draw(Object_OpenGL::program_);
}


//------------------------------------------------------------create_program-------------------------------------------------------------


int Star_OpenGL::create_program(const glm::mat4 &view, const glm::mat4 &projection)
{
    program_ = Shader("Shaders/light_shader.vs", "Shaders/light_shader.frag");

    program_.use_Program();

    glm::mat4 View = view;
    glm::mat4 Projection = projection;
    glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);

    program_.set_matrix4fv("view", View);
    program_.set_matrix4fv("projection", Projection);
    program_.set_vec3f("view_pos", view_pos);

    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    
    return 0;
}


int Object_OpenGL::create_program(const glm::mat4 &view, const glm::mat4 &projection)
{
    Object_OpenGL::program_ = Shader("Shaders/simple_shader.vs", "Shaders/simple_shader.frag");

    Object_OpenGL::program_.use_Program();

    int count_lights = Lights.size();
    Object_OpenGL::program_.set_int("count_lights", count_lights);

    for (int i = 0; i < count_lights; i++)
    {
        std::string name = "lights[" + std::to_string(i) + "]";

        glm::vec3 color;
        switch (Lights[i]->color())
        {
            case ColorType::Yellow:
            {
                color = glm::vec3(1.0f, 1.0f, 0.3f);
                break;
            }
            case ColorType::White:
            {
                color = glm::vec3(1.0f, 1.0f, 1.0f);
                break;
            }
            case ColorType::Violet:
            {
                color = glm::vec3(1.0f, 0.0f, 1.0f);
                break;
            }
            case ColorType::Blue:
            {
                color = glm::vec3(0.2f, 0.9f, 0.8f);
                break;
            }
            case ColorType::Red:
            {
                color = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            }
        }

        Object_OpenGL::program_.set_vec3f(name + ".ambient",  color);
        Object_OpenGL::program_.set_vec3f(name + ".diffuse",  color);
        Object_OpenGL::program_.set_vec3f(name + ".specular", 1.0f, 1.0f, 1.0f); 

        Object_OpenGL::program_.set_vec3f("material.specular", 0.1f, 0.1f, 0.1f);
        Object_OpenGL::program_.set_float("material.shininess", 4.0f);

        glm::vec3 light_pos = glm::vec3(Lights[i]->x(), Lights[i]->y(), 1.0f);
        Object_OpenGL::program_.set_vec3f(name + ".position", light_pos);
        Object_OpenGL::program_.set_float(name + ".constant",  1.0f);
        Object_OpenGL::program_.set_float(name + ".linear",    0.09f);
        Object_OpenGL::program_.set_float(name + ".quadratic", 0.032f);
    }

    glm::mat4 View = view;
    glm::mat4 Projection = projection;
    glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);

    Object_OpenGL::program_.set_matrix4fv("view", View);
    Object_OpenGL::program_.set_matrix4fv("projection", Projection);
    Object_OpenGL::program_.set_vec3f("view_pos", view_pos);

    return 0;
}


