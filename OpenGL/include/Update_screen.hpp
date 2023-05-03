#ifndef UPDATE_SCREEN_H
#define UPDATE_SCREEN_H


#include "Background.hpp"
#include <TextProcessor.hpp>



const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;


/// Обработчик текста входного файла
TextProcessor text_processor;

/// Массив объектов на экране
std::vector<Object_OpenGL*> obj_list;

/// Указатель на текстуру фона
Texture *background;

int ARGC;
const char** ARGV;

extern Camera camera;


/**
 * @brief Update_obj_list - функция обновления экрана после внесений изменений во входной файл
 */
void Update_obj_list();


/**
 * @brief handler - функция-обработчик сигнала SIGINT, послылаемого пользователем для обновления экрана
 */
void handler(int signo);


/**
 * @return путь к файлу с изображением фона
 */
std::string get_background_name();


//--------------------------------------Handler_to_update_obj_list-----------------------------------------
  

void handler(int signo)
{
    Update_obj_list();
}


void Update_obj_list()
{
    Lights.clear();

    text_processor.get_input(ARGC, ARGV);
    text_processor.process();
    std::cout << "\nUpdate\n";

    std::vector<Object*> OBJECT_LIST = text_processor.get_obj_list();

    for (auto elem : OBJECT_LIST)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }


    for (auto elem : OBJECT_LIST)
    {
        if (dynamic_cast<Star*>(elem))
        {
            Lights.push_back(dynamic_cast<Star*>(elem));
        }
    }

    obj_list.clear();

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection_text = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));

    //---------------------create program for all OpenGL objects--------------------


    Object_OpenGL::create_program(view, projection); 


    //----------------------------fill objects array--------------------------------


    for (auto elem : OBJECT_LIST)
    {
        switch (elem->type())
        {
            case ObjectType::Pine:
            {
                obj_list.push_back(new Pine_OpenGL(elem, view, projection));
                break;
    
            }
            case ObjectType::Star:
            {
                obj_list.push_back(new Star_OpenGL(elem, view, projection));
                break;
            }
            case ObjectType::Gift:
            {
                obj_list.push_back(new Gift_OpenGL(elem, view, projection));
                break;                
            }
            case ObjectType::Congratulation:
            {
                obj_list.push_back(new Congratulation_OpenGL(dynamic_cast<Congratulation*>(elem), view, projection_text));
                break;                
            }
            default:
            {
                std::cerr << "ERROR in switch(elem->type) while creation obj_list\n";
                exit(1);
            }
        }
    }

    //----------------------------create background--------------------------------

    std::string background_name = get_background_name();
    background = new Texture(background_name.c_str());
}


std::string get_background_name()
{
    int num_background = text_processor.get_background();

    std::ostringstream name;
    name << "../backgrounds/ng" << num_background << ".jpg";

    return name.str();
}


#endif
