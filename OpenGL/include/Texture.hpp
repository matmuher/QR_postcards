#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.hpp"


//--------------------------------------------------Texture-------------------------------------------------------


class Texture
{
	static int count;
	int num_;
    unsigned int ID_;

public:

	Texture(const char *file_name);
	void active() const;
	int get_ID() const { return ID_; };
	int get_num() const { return num_; };

};


//----------------------------------------------------------------------------------------------------------------


int Texture::count = 0;


Texture::Texture(const char *file_name)
{
	if (count > 16)
	{
		throw std::logic_error("Max count textures = 16");
	}

	num_ = count;
	count++;

    glGenTextures(1, &ID_);
    glBindTexture(GL_TEXTURE_2D, ID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    int width, height, nrChannels;
    unsigned char *image = stbi_load(file_name, &width, &height, &nrChannels, 0); 
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed download texture" << std::endl;
    }

    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, ID_);
}


void Texture::active() const
{
	glActiveTexture(GL_TEXTURE0 + num_);
    glBindTexture(GL_TEXTURE_2D, ID_);
}



#endif
