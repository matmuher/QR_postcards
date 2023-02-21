#include "Camera.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 view_pos = glm::vec3(0.0f, 0.0f, 7.0f);
Camera camera(view_pos);


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

	Shader shader_program_light("Shaders/light_shader.vs", "Shaders/light_shader.frag");
    Shader shader_program_simple("Shaders/simple_shader.vs", "Shaders/simple_shader.frag");
    
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

    //create vertex array object for CUBE

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glm::vec3 light_pos;
    glm::mat4 model_light, model(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f , 0.0f,  0.0f));
    //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    glm::mat3 norm_model(1.0f);

    glm::mat4 view = camera.get_view_matrix();

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
  
    shader_program_simple.use_Program();

    shader_program_simple.set_matrix4fv("view", view);
    shader_program_simple.set_matrix4fv("projection", projection);
    shader_program_simple.set_vec3f("view_pos", view_pos);

    shader_program_light.use_Program();

    shader_program_light.set_matrix4fv("view", view);
    shader_program_light.set_matrix4fv("projection", projection);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.get_ID()))
    {
        window.process_input();

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLfloat time_value = glfwGetTime();
        shader_program_light.use_Program();


        model_light = glm::mat4(1.0f);
        light_pos = glm::vec3(2.0f * cos(time_value), 1.0f, sin(time_value) * 2.0f);

        model_light = glm::translate(model_light, light_pos);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        shader_program_light.set_matrix4fv("model", model_light);
        
        glBindVertexArray(cubeVAO); 
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glDrawArrays(GL_TRIANGLES, 0, 36);


        model_light = glm::translate(model_light, glm::vec3(0.0f, -2.0f, 0.0f));
        shader_program_light.set_matrix4fv("model", model_light);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader_program_simple.use_Program();

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            

        norm_model = glm::mat3(glm::transpose(glm::inverse(model)));
            
        shader_program_simple.set_matrix4fv("model", model);
        shader_program_simple.set_matrix3fv("norm_model", norm_model);

        
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f), light_color = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 view_pos = camera.get_pos();

        shader_program_simple.set_vec3f("obj_color", color);
        shader_program_simple.set_vec3f("light_color", light_color);
        shader_program_simple.set_vec3f("light_pos", light_pos);

        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swap_buffers();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
 
    glfwTerminate();
    return 0;
}
