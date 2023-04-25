#ifndef WINDOW_H
#define WINDOW_H

#include "Texture.hpp"


float mix_value = 0.2f;
bool keys[1024];


//--------------------------------------------------Window-------------------------------------------------------


class Window
{
	GLFWwindow* ID_;
	int width_, height_;

public:

	Window(const char *name, int width, int height);
	void swap_buffers() const;
	void set_framebuffer_size_callback() const;
    GLFWwindow* get_ID() const { return ID_; };
    int get_width() const { return width_; };
    int get_height() const { return height_; };
};


//----------------------------------------------------------------------------------------------------------------


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


#endif
