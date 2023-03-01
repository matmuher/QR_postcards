#include "Object_OpenGL.hpp"



//void create_VAO_for_cube(unsigned int *cubeVAO, float *vertices, size_t size);


glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
Camera camera(view_pos);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window("NewYearCard", SCR_WIDTH, SCR_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
 
    std::vector<Object*> OBJECT_LIST = {
                                         new Star(-2.0f,  -1.0f, Color::WHITE),
                                         new Pine( 2.0f, 2.0f, Color::RED),
                                         new Pine(-1.0f, -1.0f, Color::RED)
                                        };

    std::vector<Object_OpenGL*> obj_list;

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);


    for (auto elem : OBJECT_LIST)
    {
        switch (elem->type())
        {
            case Object_type::PINE:
            {
                obj_list.push_back(new Pine_OpenGL(elem, view, projection));
                break;
    
            }
            case Object_type::STAR:
            {
                obj_list.push_back(new Star_OpenGL(elem, view, projection));
                break;
            }
        }
    }

    //model = glm::translate(model, glm::vec3(0.0f , 0.0f,  0.0f));
    //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    glm::vec3 pos(-1.0f, -1.0f, 0.0f), view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
    glm::mat4 model_(1.0f);
    model_ = glm::translate(model_, pos);

    glm::mat3 norm_model(1.0f);

    while (!glfwWindowShouldClose(window.get_ID()))
    {
        window.process_input();

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat time_value = glfwGetTime();

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


