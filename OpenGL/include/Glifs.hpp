#ifndef GLIFS_H
#define GLIFS_H

#include "Model.hpp"



/**
 * @brief struct Character представляет один символ текста на экране и его характеристики 
 */
struct Character
{
    /// Изоражение символа
    unsigned int TextureID;

    /// Ширина и высота символа
    glm::ivec2 size;

    /// Смещение верхней левой точки глифа      
    glm::ivec2 bearing;

    /// Горизонтальное смещение до начала следующего глифа
    long int offset;
};


/// Множество ассоциированных символов типа char с их представлением Character
std::map<GLchar, Character> Characters;

/// Массив и буфер вершин для глифов
unsigned int VAO, VBO;


//---------------------------------------Text_functions--------------------------------------------


/**
 * @brief make_glifs - функция для заполнения множества std::map<GLchar, Character> Characters
 */
void make_glifs();

/**
 * @brief Render_text - функция для создания шейдера текста s
 * 
 * @param text - текст, x, y, scale - расположение на экране и размер, относительного заданного по умолчанию
 *        color - цвет текста
 */
void Render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);


void make_glifs()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        throw std::logic_error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
    {
        throw std::logic_error("ERROR::FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
	for (unsigned char c = 0; c < 128; c++)
	{
	    // load character glyph 
	    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	    {
	        std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n" << std::endl;
	        continue;
	    }
	    
	    // generate texture
	    unsigned int texture;
	    glGenTextures(1, &texture);
	    glBindTexture(GL_TEXTURE_2D, texture);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	    
	    // set texture options
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    // now store character for later use
	    Character character = {
	        texture, 
	        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	        face->glyph->advance.x
	    };

	    Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}


void Render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    s.use_Program();
    glUniform3f(glGetUniformLocation(s.get_Program(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.offset >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


#endif
