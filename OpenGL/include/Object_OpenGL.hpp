#include "Objects.hpp"

#include "Glifs.hpp"
#include <vector>


std::vector<Star*> Lights;


//--------------------------------------------------ABSTRACT_BASE_CLASS_OBJECT_OpenGL----------------------------------------------------------


class Object_OpenGL
{

protected:

    static Shader program_;
    glm::mat4 model_;   
    Model Model_obj_;

public:

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


//-------------------------------------------------------CLASS_Congratulation_OpenGL------------------------------------------------------------------


class Congratulation_OpenGL : public Object_OpenGL
{
    static Shader program_;
    glm::vec3 color_;
    Congratulation congrat_;

public:

    Congratulation_OpenGL(Congratulation *congrat, const glm::mat4 &view, const glm::mat4 &projection);
    void draw() override;
    int create_program(const glm::mat4 &view, const glm::mat4 &projection);
    Congratulation congratulation() const { return congrat_; }
};

Shader Congratulation_OpenGL::program_;


//------------------------------------------------------------Objects_methods------------------------------------------------------------------


//-------------------------------------------------------------CONSTRUCTORS--------------------------------------------------------------------


Pine_OpenGL::Pine_OpenGL(Object *pine, const glm::mat4 &view, const glm::mat4 &projection) : pine_(*pine)
{
    Model_obj_ = Model((std::filesystem::path("../objects/Christmas_tree/Tree.obj")).c_str());

    float x_coord = -4.0f + (pine_.x() / 50.0f) * 4.0f;
    float y_coord = -3.0f + (pine_.y() / 50.0f) * 3.0f;

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, glm::vec3(x_coord, y_coord, 0.0f));
    model_ = glm::scale(model_, glm::vec3(0.5f * pine_.size()));
    
    std::cout << "pine coords: " << pine_.x() << "  " << pine_.y() << std::endl;

}


Congratulation_OpenGL::Congratulation_OpenGL(Congratulation *congrat, const glm::mat4 &view, const glm::mat4 &projection) : congrat_(*congrat)
{
    static int init_program = create_program(view, projection);
   
    program_.use_Program();
  
    switch (congrat_.color())
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
    
    std::cout << "congratulation coords: " << congrat_.x() << "  " << congrat_.y() << std::endl;

}


Gift_OpenGL::Gift_OpenGL(Object *gift, const glm::mat4 &view, const glm::mat4 &projection) : gift_(*gift)
{
    Model_obj_ = Model((std::filesystem::path("../objects/gift/gift.obj")).c_str());

    float x_coord = -4.0f + (gift_.x() / 50.0f) * 4.0f;
    float y_coord = -3.0f + (gift_.y() / 50.0f) * 3.0f;

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, glm::vec3(x_coord, y_coord, 0.0f));
    model_ = glm::scale(model_, glm::vec3(0.02f * gift_.size()));
    model_ = glm::rotate(model_, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    std::cout << "gift coords: " << gift_.x() << "  " << gift_.y() << std::endl;

}


Star_OpenGL::Star_OpenGL(Object *star, const glm::mat4 &view, const glm::mat4 &projection) : star_(*star)
{
	static int init_program = create_program(view, projection);

    Model_obj_ = Model((std::filesystem::path("../objects/star/star.obj")).c_str());

    program_.use_Program();

    float x_coord = -2.2f + (star_.x() / 50.0f) * 2.2f;
    float y_coord = -1.8f + (star_.y() / 50.0f) * 1.8f;
  
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
    model_ = glm::translate(model_, glm::vec3(x_coord, y_coord, 3.0f));
    model_ = glm::scale(model_, glm::vec3(0.04f * star_.size()));
    model_ = glm::rotate(model_, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    std::cout << "star coords: " << star_.x() << " " << star_.y() << std::endl;
}


//-----------------------------------------------------------------Draw()-------------------------------------------------------------------


void Star_OpenGL::draw()
{
    program_.use_Program();
    
    model_ = glm::rotate(model_, glm::radians(1.0f * star_.action_intensity()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));

    program_.set_matrix4fv("model", model_);
    program_.set_matrix3fv("norm_model", norm_model);
    program_.set_vec3f("light_color", color_);
 
    Model_obj_.draw(program_);

}


void Pine_OpenGL::draw()
{
	Object_OpenGL::program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f * pine_.action_intensity()), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));

    Object_OpenGL::program_.set_matrix4fv("model", model_);
    Object_OpenGL::program_.set_matrix3fv("norm_model", norm_model);
    Object_OpenGL::program_.set_float("ambientStrength", 0.2f);

    Model_obj_.draw(Object_OpenGL::program_);
}


void Congratulation_OpenGL::draw()
{
    program_.use_Program();

    std::vector<Line> lines = congrat_.get_lines();
    int y = congrat_.y();

    for (auto& line : lines)
    {
        Render_text(program_, line.get_msg(), congrat_.x() * 10.0f, y * 7.0f, congrat_.size() / 3.0f, color_);
        y -= (2.0f * congrat_.size());
    }
}      


void Gift_OpenGL::draw()
{
    Object_OpenGL::program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f * gift_.action_intensity()), glm::vec3(0.0f, 0.0f, 1.0f));

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


int Congratulation_OpenGL::create_program(const glm::mat4 &view, const glm::mat4 &projection)
{
    program_ = Shader("Shaders/text_shader.vs", "Shaders/text_shader.frag");
    program_.use_Program();

    make_glifs();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 Projection = projection;
    program_.set_matrix4fv("projection", Projection);
    
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

        float x_light = -2.2f + (Lights[i]->x() / 50.0f) * 2.2f;
        float y_light = -1.8f + (Lights[i]->y() / 50.0f) * 1.8f;
        

        glm::vec3 light_pos = glm::vec3(x_light, y_light, 1.0f);
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


