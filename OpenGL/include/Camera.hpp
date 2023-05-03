#ifndef CAMERA_H
#define CAMERA_H

#include "Window.hpp"




/**
 * @brief class Camera представляет направление, с которого осуществляется
 *        наблюдение на сцену
 */
class Camera
{
	double lastX_, lastY_;

	float yaw = -90.0f;
	float pitch = 0.0f;

    /// Позиция
	glm::vec3 pos_;

    /// Вектор, по которому направлена камера
	glm::vec3 front_;

    /// Направление верха камеры
	glm::vec3 up_;

    /// Скорость при наличии движения
	float camera_speed = 3.0f;

    /// Чувствительность
	float sensitivity = 0.05f;

public:

	Camera(const glm::vec3 &pos, const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f)) : pos_(pos), front_(glm::vec3(0.0f, 0.0f, -1.0f)), up_(up){};

    /**
     * @brief process_mouse_callback - функция-обработчик нажатий мыши
     */
	void process_mouse_callback(double xpos, double ypos);

    /**
     * @brief make_move - функция для осуществления перемещений камеры
     */   
	void make_move(float delta_time);

	glm::mat4 get_view_matrix() const;
	void set_firstXY(GLFWwindow* window);
    glm::vec3 get_pos() const { return pos_; };
};



void Camera::set_firstXY(GLFWwindow* window)
{
	glfwGetCursorPos(window, &lastX_, &lastY_);
}


void Camera::process_mouse_callback(double xpos, double ypos)
{

    float xoffset = xpos - lastX_;
    float yoffset = lastY_ - ypos;
    lastX_ = xpos;
    lastY_ = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front_ = glm::normalize(front);
}


void Camera::make_move(float delta_time)
{
    float real_speed = camera_speed * delta_time;

    if (keys[GLFW_KEY_I])
    {
        pos_ += real_speed * front_;
    }
    if (keys[GLFW_KEY_K])
    {
        pos_ -= real_speed * front_;
    }
    if (keys[GLFW_KEY_J])
    {
        pos_ -= glm::normalize(glm::cross(front_, up_)) * real_speed;
    }
    if (keys[GLFW_KEY_L])
    {
        pos_ += glm::normalize(glm::cross(front_, up_)) * real_speed;
    }
}


glm::mat4 Camera::get_view_matrix() const
{
	return glm::lookAt(pos_, pos_ + front_, up_);
}

#endif
