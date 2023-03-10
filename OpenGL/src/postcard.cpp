#include "Object_OpenGL.hpp"

        


glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
Camera camera(view_pos);
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;


unsigned int create_VAO_background();


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
                                    
                                         new Pine( 2.0f, -1.0f, Color::RED),
                                         new Pine(0.0f, -2.0f, Color::WHITE),
                                         new Pine(-2.0f, -1.0f, Color::WHITE),
                                         new Star(0.0f, 1.0f, Color::BLUE),
                                         new Star(1.0f, 1.0f, Color::RED),
                                         new Star(-1.0f, 1.0f, Color::WHITE),
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

    glm::mat4 model_background(1.0f);
    model_background = glm::scale(model_background, glm::vec3(13.0f));
    model_background = glm::rotate(model_background, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
 

    Texture background("../backgrounds/ng1.jpg");

    unsigned int VAO_background = create_VAO_background();


    //--------------------------------Game_loop------------------------------------


    while (!glfwWindowShouldClose(window.get_ID()))
    {
        window.process_input();

        glClearColor(0.7, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat time_value = glfwGetTime();

        for (auto elem : obj_list)
        {
            elem->draw();
        }

        background.active();

        (obj_list[0]->program()).use_Program();

        (obj_list[0]->program()).set_matrix4fv("model", model_background);
        glm::mat3 norm_model = glm::mat3(glm::transpose(glm::inverse(model_background)));
        (obj_list[0]->program()).set_matrix3fv("norm_model", norm_model);
        (obj_list[0]->program()).set_float("ambientStrength", 4.0f);
        
        glBindVertexArray(VAO_background);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


unsigned int create_VAO_background()
{
    float vertices[] = {
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    -0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
    -0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
     0.85f,  0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
     0.85f, -0.64f, -1.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

    return VAO;
}
