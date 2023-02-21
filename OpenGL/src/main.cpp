#include "Camera.hpp"



void mouse_callback(GLFWwindow* window, double xpos, double ypos);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window("LearnOpenGL", SCR_WIDTH, SCR_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    Shader shaderProgramPurple("Shaders/shader.vs", "Shaders/shader.frag");
    Shader shaderProgramLight("Shaders/light_shader.vs", "Shaders/light_shader.frag");
    Shader shaderProgramSimple("Shaders/simple_shader.vs", "Shaders/simple_shader.frag");
    

    Texture texture1("ng4.jpg");
    Texture texture2("ng1.jpg");

    shaderProgramPurple.use_Program();

    shaderProgramPurple.set_int("ourTexture1", 0);
    shaderProgramPurple.set_int("ourTexture2", 1);


    float firstTriangle[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

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

    unsigned int indices[] = {
        0, 1, 3,  
        1, 2, 3,
    };


    //VAO for cubs


    unsigned int VBOs[2], VAOs[2], EBO;
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    //glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0); 


    glm::mat4 trans = glm::mat4(1.0f), trans1, trans2;
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans2 = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
  

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    texture1.active();
    texture2.active();

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cube_positions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };


    //VAO for lights

    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::vec3 lightPos(1.0f, 1.0f, 4.0f);

    glm::mat4 model_light = glm::mat4(1.0f);


    glfwSetInputMode(window.get_ID(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.get_ID(), mouse_callback);

    float delta_time = 0.0f, last_time = 0.0f;
    camera.set_firstXY(window.get_ID());

    while (!glfwWindowShouldClose(window.get_ID()))
    {
        window.process_input();

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgramPurple.set_float("mix_value", mix_value);

        shaderProgramPurple.use_Program();

        GLfloat timeValue = glfwGetTime();

        delta_time = timeValue - last_time;
        last_time = timeValue;

        GLfloat offset = (sin(timeValue) / 2) + 0.5;

        trans1 = glm::rotate(trans, (GLfloat)glfwGetTime() * 3.0f, glm::vec3(0.0, 0.0, 1.0));
        //trans1 = glm::scale(trans1, glm::vec3(sin(timeValue), sin(timeValue), sin(timeValue)));
        
        glm::mat4 view = camera.get_view_matrix();

        #if 1

        shaderProgramPurple.set_matrix4fv("transform", trans1);
        shaderProgramPurple.set_matrix4fv("view", view);
        shaderProgramPurple.set_matrix4fv("projection", projection);
        shaderProgramPurple.set_float("offset", offset);
        shaderProgramPurple.set_float("num_triangle", 1);

        glBindVertexArray(VAOs[0]); 
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

    

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cube_positions[i]);

            float angle = 10.0 * i;

            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            
            shaderProgramPurple.set_matrix4fv("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        #endif

        shaderProgramLight.use_Program();

        shaderProgramLight.set_matrix4fv("view", view);
        shaderProgramLight.set_matrix4fv("projection", projection);

        model_light = glm::mat4(1.0f);
        lightPos = glm::vec3(2.0f * cos(timeValue), 1.0f, sin(timeValue) * 2.0f);

        model_light = glm::translate(model_light, lightPos);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        shaderProgramLight.set_matrix4fv("model", model_light);
        
        glBindVertexArray(lightVAO); 
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glDrawArrays(GL_TRIANGLES, 0, 36);


        model_light = glm::translate(model_light, glm::vec3(0.0f, -2.0f, 0.0f));
        shaderProgramLight.set_matrix4fv("model", model_light);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shaderProgramSimple.use_Program();

        glm::mat4 model(1.0f);
        glm::mat3 norm_model(1.0f);
        model = glm::translate(model, glm::vec3(3.0f * cos(timeValue), 0.0f, sin(timeValue) * 3.0f));
        //model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        norm_model = glm::mat3(glm::transpose(glm::inverse(model)));
            
        shaderProgramSimple.set_matrix4fv("model", model);
        shaderProgramSimple.set_matrix3fv("norm_model", norm_model);
        shaderProgramSimple.set_matrix4fv("view", view);
        shaderProgramSimple.set_matrix4fv("projection", projection);
        
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f), light_color = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 view_pos = camera.get_pos();

        shaderProgramSimple.set_vec3f("view_pos", view_pos);
        shaderProgramSimple.set_vec3f("obj_color", color);
        shaderProgramSimple.set_vec3f("light_color", light_color);
        shaderProgramSimple.set_vec3f("light_pos", lightPos);

        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // shaderProgramPurple.set_matrix4fv("transform", trans2);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swap_buffers();
        glfwPollEvents();
        camera.make_move(delta_time);
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.process_mouse_callback(xpos, ypos);
}
