#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


class Shader
{
    GLuint Program;

public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ~Shader();
    void use_Program() const;
    unsigned int get_Program() const { return Program; };
    void check_compile_errors(unsigned int shader, std::string type) const;
    void set_float(const char *variable, float value) const;
    void set_int(const char *variable, float value) const;
    void set_matrix4fv(const char *matrix_name, glm::mat4 &matrix) const;
    void set_matrix3fv(const char *matrix_name, glm::mat3 &matrix) const;
    void set_vec3f(const char *vec_name, glm::vec3 &vec) const;

};


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    check_compile_errors(vertexShader, "VERTEX");

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    check_compile_errors(fragmentShader, "FRAGMENT");

    // link shaders

    Program = glCreateProgram();

    glAttachShader(Program, vertexShader);
    glAttachShader(Program, fragmentShader);
    glLinkProgram(Program);

    check_compile_errors(Program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	
}


void Shader::check_compile_errors(unsigned int shader, std::string type) const
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


Shader::~Shader()
{
    //glDeleteProgram(Program);
}


void Shader::use_Program() const
{
    glUseProgram(Program);
}


void Shader::set_float(const char *variable, float value) const
{
    int Var = glGetUniformLocation(Program, variable);
    glUniform1f(Var, value);
}


void Shader::set_int(const char *variable, float value) const
{
    int Var = glGetUniformLocation(Program, variable);
    glUniform1i(Var, value);
}


void Shader::set_matrix4fv(const char *matrix_name, glm::mat4 &matrix) const
{
    unsigned int matrix_id = glGetUniformLocation(Program, matrix_name);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::set_matrix3fv(const char *matrix_name, glm::mat3 &matrix) const
{
    unsigned int matrix_id = glGetUniformLocation(Program, matrix_name);
    glUniformMatrix3fv(matrix_id, 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::set_vec3f(const char *vec_name, glm::vec3 &vec) const
{
    unsigned int vec_id = glGetUniformLocation(Program, vec_name);
    glUniform3f(vec_id,  vec.x, vec.y, vec.z);
}


#endif
