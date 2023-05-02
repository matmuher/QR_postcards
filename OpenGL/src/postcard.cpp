#include "Object_OpenGL.hpp"
#include <TextProcessor.hpp>
#include <iostream>
#include <sstream>
#include <signal.h>

// Это документация функции Update_obj_list
void Update_obj_list();
void handler(int signo);
std::string get_background_name();
std::pair<unsigned int, unsigned int> create_VAO_background();
void draw_background(Texture &background, glm::mat4 &model_background, Shader &program);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);


glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
Camera camera(view_pos);

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

TextProcessor text_processor;
std::vector<Object_OpenGL*> obj_list;
Texture *background;

int ARGC;
const char** ARGV;


//----------------------------------------------main---------------------------------------------------


int main(int argc, const char* argv[])
{
    glfwInit();
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);//comment this in last version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ARGC = argc;
    ARGV = argv;

    Window window("NewYearCard", SCR_WIDTH, SCR_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Update_obj_list();
 
    glm::mat4 model_background(1.0f);
    model_background = glm::scale(model_background, glm::vec3(13.0f));
    model_background = glm::rotate(model_background, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    //--------------------------------Game_loop------------------------------------
  

    signal(SIGINT, handler); //if you update file with program send Ctrl+C


    while (!glfwWindowShouldClose(window.get_ID()))
    {
        //window.process_input();

        glClearColor(0.7, 0.5, 0.5, 1);
        glClearColor(1.0, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat time_value = glfwGetTime();

        draw_background(*background, model_background, Object_OpenGL::program());

        for (auto elem : obj_list)
        {
            elem->draw();
        }
 
        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


//--------------------------------------Handler_to_update_obj_list-----------------------------------------
  

void handler(int signo)
{
    Update_obj_list();
}


void Update_obj_list()
{
    Lights.clear();

    text_processor.get_input(ARGC, ARGV);
    text_processor.process();
    std::cout << "\nUpdate\n";

    std::vector<Object*> OBJECT_LIST = text_processor.get_obj_list();

    for (auto elem : OBJECT_LIST)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }


    for (auto elem : OBJECT_LIST)
    {
        if (dynamic_cast<Star*>(elem))
        {
            Lights.push_back(dynamic_cast<Star*>(elem));
        }
    }

    obj_list.clear();

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection_text = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));

    //---------------------create program for all OpenGL objects--------------------


    Object_OpenGL::create_program(view, projection); 


    //----------------------------fill objects array--------------------------------


    for (auto elem : OBJECT_LIST)
    {
        switch (elem->type())
        {
            case ObjectType::Pine:
            {
                obj_list.push_back(new Pine_OpenGL(elem, view, projection));
                break;
    
            }
            case ObjectType::Star:
            {
                obj_list.push_back(new Star_OpenGL(elem, view, projection));
                break;
            }
            case ObjectType::Gift:
            {
                obj_list.push_back(new Gift_OpenGL(elem, view, projection));
                break;                
            }
            case ObjectType::Congratulation:
            {
                obj_list.push_back(new Congratulation_OpenGL(dynamic_cast<Congratulation*>(elem), view, projection_text));
                break;                
            }
            default:
            {
                std::cerr << "ERROR in switch(elem->type) while creation obj_list\n";
                exit(1);
            }
        }
    }

    //----------------------------create background--------------------------------

    std::string background_name = get_background_name();
    background = new Texture(background_name.c_str());
}


//----------------------------------------------Functions---------------------------------------------------


std::pair<unsigned int, unsigned int> create_VAO_background()
{
    float vertices[] = {
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    -0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
     0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    };

    unsigned int VBO_back, VAO_back;
    glGenVertexArrays(1, &VAO_back);
    glGenBuffers(1, &VBO_back);

    glBindVertexArray(VAO_back);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_back);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
 
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return std::make_pair(VAO_back, VBO_back);
}


std::string get_background_name()
{
    int num_background = text_processor.get_background();

    std::ostringstream name;
    name << "../backgrounds/ng" << num_background << ".jpg";

    return name.str();
}


void draw_background(Texture &background, glm::mat4 &model_background, Shader &program)
{
    static std::pair<unsigned int, unsigned int> V_background = create_VAO_background();

    int VAO_background = V_background.first;
    int VBO_background = V_background.second;

    background.active();
    program.use_Program();

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_background)));

    program.set_matrix4fv("model", model_background);
    program.set_matrix3fv("norm_model", norm_model);
    program.set_float("ambientStrength", 4.0f);
    
    glBindVertexArray(VAO_background);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
