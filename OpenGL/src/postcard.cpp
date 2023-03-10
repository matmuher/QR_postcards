#include "Object_OpenGL.hpp"

        


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
                                    
                                         new Pine( 2.0f, -1.0f, Color::RED),
                                         new Pine(0.0f, -2.0f, Color::WHITE),
                                         new Pine(-2.0f, -1.0f, Color::WHITE),
                                         new Star(-1.0f,  -1.0f, Color::VIOLET),
                                         new Star(2.0f,  -1.0f, Color::BLUE),
                                         new Star(1.0f, 1.0f, Color::WHITE),
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

    //model = glm::translate(model, glm::vec3(0.0f , 0.0f,  0.0f));
    //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));


    //-----------------------------------------------------
    Shader tex_shader("Shaders/tex_shader.vs", "Shaders/tex_shader.frag");
    

    tex_shader.use_Program();

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(13.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    tex_shader.set_matrix4fv("view", view);
    tex_shader.set_matrix4fv("projection", projection);
    tex_shader.set_matrix4fv("model", model);


    float firstTriangle[] = {
    -0.6f, -0.5f, -1.0f,  0.0f, 0.0f,
     0.6f, -0.5f, -1.0f,  1.0f, 0.0f,
     0.6f,  0.5f, -1.0f,  1.0f, 1.0f,
     0.6f,  0.5f, -1.0f,  1.0f, 1.0f,
    -0.6f,  0.5f, -1.0f,  0.0f, 1.0f,
    -0.6f, -0.5f, -1.0f,  0.0f, 0.0f,
};
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
    // texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load((std::filesystem::path("../backgrounds/ng4.jpg")).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    //-------------------------------------------------------

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

        
        glBindTexture(GL_TEXTURE_2D, texture);
        tex_shader.use_Program();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


