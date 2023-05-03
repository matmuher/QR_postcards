#ifndef BACKGROUND_H
#define BACKGROUND_H


#include "Object_OpenGL.hpp"


/**
 * @brief create_VAO_background - функция для создания объекта вершинного массива фона для
 *        упрощения смены вершинных данных и конфигураций атрибутов
 * 
 * @return пара из идентификаторов объекта вершинного массива фона(VAO) и объекта вершинного буфера фона(VBO)
 */
std::pair<unsigned int, unsigned int> create_VAO_background();


/**
 * @brief draw_background - функция для отображения фона на экран
 */
void draw_background(Texture &background, glm::mat4 &model_background, Shader &program);



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


#endif
