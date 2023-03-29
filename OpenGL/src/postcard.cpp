#include "Object_OpenGL.hpp"

<<<<<<< HEAD
        
=======
>>>>>>> origin/ksusha_210223_graphics


glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
Camera camera(view_pos);
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;


<<<<<<< HEAD
unsigned int create_VAO_background();
void draw_background(glm::mat4 &model_background, Shader &program);
=======
std::pair<unsigned int, unsigned int> create_VAO_background();
void draw_background(glm::mat4 &model_background, Shader &program);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);
>>>>>>> origin/ksusha_210223_graphics


int main()
{
    glfwInit();
<<<<<<< HEAD
=======
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);//comment this in last version
>>>>>>> origin/ksusha_210223_graphics
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window("NewYearCard", SCR_WIDTH, SCR_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
<<<<<<< HEAD
    }


    glEnable(GL_DEPTH_TEST);
 
    std::vector<Object*> OBJECT_LIST = {
                                    
                                         new Pine( 2.0, -1.0, 1, ColorType::Red, 0),
                                         new Pine(0.0, -2.0, 2, ColorType::White, 2),
                                         new Gift(2.0, -1.0, 3, ColorType::White, 5),
                                         new Pine(-2.0, -1.0, 3, ColorType::White, 4),
                                         new Star(0.0, 1.0, 1, ColorType::Yellow, 2),
                                         new Star(1.0, 1.0, 2, ColorType::Yellow, 1),
                                         new Gift(-2.0, -1.0, 1, ColorType::White, 0),
                                         new Star(-1.0, 1.0, 1, ColorType::Yellow, 0),
=======
        exit(1);
    }

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
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

    //пользователь задает положение любого объекта x : (0, 100), y : (0, 100)
    //размер всего, интенсивность(кроме текста) : (1, 5)
 
    std::vector<Object*> OBJECT_LIST = {
                                    
                                         new Pine(40.0, 50.0, 1, ColorType::Red, 0),
                                         new Pine(70.0, 40.0, 2, ColorType::White, 2),
                                         new Gift(90.0, 25.0, 3, ColorType::White, 2),
                                         new Pine(20.0, 10.0, 3, ColorType::White, 4),
                                         new Star(50.0, 80.0, 1, ColorType::Yellow, 2),
                                         new Star(80.0, 80.0, 2, ColorType::Yellow, 1),
                                         new Gift(80.0, 30.0, 2, ColorType::White, 0),
                                         new Star(10.0, 75.0, 1, ColorType::Yellow, 0),
                                         new Congratulation(10.0, 20.0, 3, ColorType::White, 0),
>>>>>>> origin/ksusha_210223_graphics
                                        };

    for (auto elem : OBJECT_LIST)
    {
        if (dynamic_cast<Star*>(elem))
        {
            Lights.push_back(dynamic_cast<Star*>(elem));
        }
    }

    std::vector<Object_OpenGL*> obj_list;

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
<<<<<<< HEAD

=======
    glm::mat4 projection_text = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
>>>>>>> origin/ksusha_210223_graphics

    Object_OpenGL::create_program(view, projection); //create program for all OpenGL objects 


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
<<<<<<< HEAD
=======
            case ObjectType::Congratulation:
            {
                Line line("Happy New Year!");
                dynamic_cast<Congratulation*>(elem)->add_line(line);
                Line line2("Matvey!");
                dynamic_cast<Congratulation*>(elem)->add_line(line2);
                obj_list.push_back(new Congratulation_OpenGL(dynamic_cast<Congratulation*>(elem), view, projection_text));
                break;                
            }
>>>>>>> origin/ksusha_210223_graphics
            default:
            {
                std::cerr << "ERROR in switch(elem->type) while creation obj_list\n";
                exit(1);
            }
        }
    }

    glm::mat4 model_background(1.0f);
    model_background = glm::scale(model_background, glm::vec3(13.0f));
    model_background = glm::rotate(model_background, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
<<<<<<< HEAD
 

    //--------------------------------Game_loop------------------------------------

=======


    //--------------------------------Game_loop------------------------------------
>>>>>>> origin/ksusha_210223_graphics

    while (!glfwWindowShouldClose(window.get_ID()))
    {
        window.process_input();

<<<<<<< HEAD
        glClearColor(0.7, 0.5, 0.5, 1);
=======
        glClearColor(1.0, 0.1, 0.1, 1);
>>>>>>> origin/ksusha_210223_graphics
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat time_value = glfwGetTime();

        for (auto elem : obj_list)
        {
            elem->draw();
        }

        draw_background(model_background, Object_OpenGL::program());
<<<<<<< HEAD

=======
 
>>>>>>> origin/ksusha_210223_graphics
        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


<<<<<<< HEAD
unsigned int create_VAO_background()
=======
std::pair<unsigned int, unsigned int> create_VAO_background()
>>>>>>> origin/ksusha_210223_graphics
{
    float vertices[] = {
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    -0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
     0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    };

<<<<<<< HEAD
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
=======
    unsigned int VBO_back, VAO_back;
    glGenVertexArrays(1, &VAO_back);
    glGenBuffers(1, &VBO_back);

    glBindVertexArray(VAO_back);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_back);
>>>>>>> origin/ksusha_210223_graphics
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

<<<<<<< HEAD
    return VAO;
=======
    return std::make_pair(VAO_back, VBO_back);
>>>>>>> origin/ksusha_210223_graphics
}


void draw_background(glm::mat4 &model_background, Shader &program)
{
    static Texture background("../backgrounds/ng1.jpg");
<<<<<<< HEAD
    static unsigned int VAO_background = create_VAO_background();

    background.active();

=======
    static std::pair<unsigned int, unsigned int> V_background = create_VAO_background();

    int VAO_background = V_background.first;
    int VBO_background = V_background.second;

    background.active();
>>>>>>> origin/ksusha_210223_graphics
    program.use_Program();

    glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_background)));

    program.set_matrix4fv("model", model_background);
    program.set_matrix3fv("norm_model", norm_model);
    program.set_float("ambientStrength", 4.0f);
<<<<<<< HEAD
    glBindVertexArray(VAO_background);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}
=======
    
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
>>>>>>> origin/ksusha_210223_graphics
