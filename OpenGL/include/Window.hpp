#ifndef WINDOW_H
#define WINDOW_H

#include "Texture.hpp"


float mix_value = 0.2f;
bool keys[1024];




class Window
{
	GLFWwindow* ID_;
	int width_, height_;

public:

	Window(const char *name, int width, int height);
	void swap_buffers() const;
	void process_input() const;
	void set_framebuffer_size_callback() const;
    GLFWwindow* get_ID() const { return ID_; };
    int get_width() const { return width_; };
    int get_height() const { return height_; };
};


Window::Window(const char *name, int width, int height) : width_(width), height_(height_)
{
    ID_ = glfwCreateWindow(width, height, name, NULL, NULL);

    if (ID_ == NULL)
    {
    	glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window"); 
    }

    glfwMakeContextCurrent(ID_);

}


void Window::set_framebuffer_size_callback() const
{
	glViewport(0, 0, width_, height_);
}


void Window::swap_buffers() const
{
	glfwSwapBuffers(ID_);
}


void Window::process_input() const
{

    if (glfwGetKey(ID_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ID_, true);
    }

    if (glfwGetKey(ID_, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mix_value += 0.01f; 
        if(mix_value >= 1.0f)
        {
            mix_value = 1.0f;
        }
    }

    if (glfwGetKey(ID_, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mix_value -= 0.01f; 
        if (mix_value <= 0.0f)
        {
            mix_value = 0.0f;
        }
    }

    if (glfwGetKey(ID_, GLFW_KEY_K) == GLFW_PRESS)
    {
        keys[GLFW_KEY_K] = true;
    }
    else
    {
        keys[GLFW_KEY_K] = false;
    }

    if (glfwGetKey(ID_, GLFW_KEY_J) == GLFW_PRESS)
    {
        keys[GLFW_KEY_J] = true;
    }
    else
    {
        keys[GLFW_KEY_J] = false;
    }

    if (glfwGetKey(ID_, GLFW_KEY_L) == GLFW_PRESS)
    {
        keys[GLFW_KEY_L] = true;
    }
    else
    {
        keys[GLFW_KEY_L] = false;
    }

    if (glfwGetKey(ID_, GLFW_KEY_I) == GLFW_PRESS)
    {
        keys[GLFW_KEY_I] = true;
    }
    else
    {
        keys[GLFW_KEY_I] = false;
    }
}


#endif
