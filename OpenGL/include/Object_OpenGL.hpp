#include "Object.hpp"
#include "Camera.hpp"
#include <vector>


int create_VAO_and_shader_for_cube(unsigned int *cubeVAO, Shader *program);
std::vector<Star*> Lights;

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



//--------------------------------------------------ABSTRACT_BASE_CLASS_OBJECT_OpenGL----------------------------------------------------------


class Object_OpenGL
{

protected:

    glm::mat4 model_, view_, projection_;
    glm::vec3 color_;    

public:

	//Object_OpenGL(Object &object) : Object(object) {};
    Object_OpenGL() = default;
	virtual ~Object_OpenGL() = default;

	virtual void draw() = 0;

    virtual int create_VAO() = 0;

};


//-----------------------------------------------------------CLASS_PINE_OpenGL-----------------------------------------------------------------


class Pine_OpenGL final : public Object_OpenGL
{
	static unsigned int VAO_;
    static unsigned int VBO_; // get_VAO() { return Pine_VAO; } // unsigned* VAO_ptr; // VAO.cpp: PineVAO = 5; //  here.cpp: extern unsigned PinVAO;
	static Shader program_;
    Pine pine_;

public:

	Pine_OpenGL(Object *pine, const glm::mat4 &view, const glm::mat4 &projection); // VAO_ptr{&PineVAO}
	~Pine_OpenGL();
	void draw() override;
    int create_VAO() override;

};

unsigned int Pine_OpenGL::VAO_, Pine_OpenGL::VBO_;
Shader Pine_OpenGL::program_;

//-----------------------------------------------------------CLASS_STAR_OpenGL-----------------------------------------------------------------



class Star_OpenGL final : public Object_OpenGL
{
	static unsigned int VAO_;
    static unsigned int VBO_;
	static Shader program_;
    Star star_;

public:

	Star_OpenGL(Object *star, const glm::mat4 &view, const glm::mat4 &projection);
	~Star_OpenGL();
    void draw() override;
    int create_VAO() override;

};

unsigned int Star_OpenGL::VAO_, Star_OpenGL::VBO_;
Shader Star_OpenGL::program_;


//------------------------------------------------------------Objects_methods------------------------------------------------------------------


//-------------------------------------------------------------CONSTRUCTORS--------------------------------------------------------------------


Pine_OpenGL::Pine_OpenGL(Object *pine, const glm::mat4 &view, const glm::mat4 &projection) : pine_(*pine)
{
	program_ = Shader("Shaders/simple_shader.vs", "Shaders/simple_shader.frag");

    static int init_VAO = create_VAO();

    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f), view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
           
    switch (pine_.color())
    {
        case Color::WHITE:
        {
            color_ = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        }
        case Color::VIOLET:
        {
            color_ = glm::vec3(1.0f, 0.0f, 1.0f);
            break;
        }
        case Color::BLUE:
        {
            color_ = glm::vec3(0.0f, 0.1f, 0.1f);
            break;
        }
        case Color::RED:
        {
            color_ = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }
    }

    program_.use_Program();

    view_ = view;
    projection_ = projection;

    program_.set_int("count_lights", Lights.size());
    program_.set_vec3f("light.ambient",  0.4f, 0.4f, 0.4f);
    program_.set_vec3f("light.diffuse",  0.7f, 0.7f, 0.7f);
    program_.set_vec3f("light.specular", 1.0f, 1.0f, 1.0f); 

    glm::vec3 light_pos = glm::vec3(Lights[0]->x(), Lights[0]->y(), 3.0f);
    program_.set_vec3f("light.position", light_pos);
    program_.set_float("light.constant",  1.0f);
    program_.set_float("light.linear",    0.09f);
    program_.set_float("light.quadratic", 0.032f);

    program_.set_matrix4fv("view", view_);
    program_.set_matrix4fv("projection", projection_);
    program_.set_vec3f("view_pos", view_pos);

    model_ = glm::mat4(1.0f);
	
    model_ = glm::translate(model_, glm::vec3(pine_.x(), pine_.y(), 0.0f));
    
    std::cout << "pine coords: " << pine_.x() << "  " << pine_.y() << std::endl;

}


Pine_OpenGL::~Pine_OpenGL()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}
    

Star_OpenGL::Star_OpenGL(Object *star, const glm::mat4 &view, const glm::mat4 &projection) : star_(*star)
{
	program_ = Shader("Shaders/light_shader.vs", "Shaders/light_shader.frag");

	static int init_VAO = create_VAO();
    
    program_.use_Program();

    view_ = view;
    projection_ = projection;
  
    switch (star_.color())
    {
        case Color::WHITE:
        {
            color_ = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        }
        case Color::VIOLET:
        {
            color_ = glm::vec3(1.0f, 0.0f, 1.0f);
            break;
        }
        case Color::BLUE:
        {
            color_ = glm::vec3(0.0f, 1.0f, 1.0f);
            break;
        }
        case Color::RED:
        {
            color_ = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }
    }

    program_.set_matrix4fv("view", view_);
    program_.set_matrix4fv("projection", projection_);
    
    glm::vec3 light_pos = glm::vec3(star_.x(), star_.y(), 3.0f);

    std::cout << "star coords: " << star_.x() << " " << star_.y() << std::endl;

    Lights.push_back(&star_);

    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, light_pos);

    model_ = glm::scale(model_, glm::vec3(0.2f));

    program_.set_matrix4fv("model", model_);
   
}


Star_OpenGL::~Star_OpenGL()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}


//-----------------------------------------------------------------Draw()-------------------------------------------------------------------


void Star_OpenGL::draw()
{
    program_.use_Program();

    program_.set_matrix4fv("model", model_);
    program_.set_vec3f("light_color", color_);
  
    glBindVertexArray(VAO_); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glDrawArrays(GL_TRIANGLES, 0, 36);

}


void Pine_OpenGL::draw()
{
	program_.use_Program();

    model_ = glm::rotate(model_, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_)));
       
    
    program_.set_vec3f("material.ambient",  1.0f, 0.5f, 0.31f);
    program_.set_vec3f("material.diffuse",  1.0f, 0.5f, 0.31f);
    program_.set_vec3f("material.specular", 0.5f, 0.5f, 0.5f);
    program_.set_float("material.shininess", 32.0f);

    program_.set_matrix4fv("model", model_);
    program_.set_matrix3fv("norm_model", norm_model);

    glBindVertexArray(VAO_); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 36);

}


//------------------------------------------------------------create_VAO-------------------------------------------------------------


int Pine_OpenGL::create_VAO()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return 0;
}


int Star_OpenGL::create_VAO()
{ 
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return 0;
}

