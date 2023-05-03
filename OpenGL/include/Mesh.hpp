#ifndef MESH_H
#define MESH_H


#include "Camera.hpp"


/**
 * @brief struct Vertex представляет вершину, ассоциированную с текстурой
 */
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};



/**
 * @brief struct Texture_Mesh представляет текстуры, ассоциированные с сеткой
 */
struct Texture_Mesh
{
    unsigned int id;
    std::string type;
    aiString path;
}; 


//--------------------------------------------------Mesh--------------------------------------------------------


/**
 * @brief class Mesh представляет сетку - массив вершин с их текстурами
 */
class Mesh
{
    /// Идентификаторы массива вершин, буфера вершин, индексов вершин 
    unsigned int VAO_, VBO_, EBO_;

    /**
     * @brief setupMesh - функция для установки атрибутов вершин
     */
    void setupMesh();

public:

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture_Mesh> textures_;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture_Mesh> textures);

    /**
     * @brief draw - функция для отрисовки данной сетки на экране
     * 
     * @param shader - шейдерная программа для отрисовки
     */    
    void draw(Shader shader);

};


//---------------------------------------------------------------------------------------------------------------



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture_Mesh> textures)
{
    vertices_ = vertices;
   	indices_ = indices;
    textures_ = textures;

    setupMesh();
};


void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
  
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), 
                 &indices_[0], GL_STATIC_DRAW);


    	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  


    glBindVertexArray(0);
}


void Mesh::draw(Shader shader) 
{
    int diffuseNr = 1;
    int specularNr = 1;

    for(int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        
        std::stringstream ss;
        std::string number;

        std::string name = textures_[i].type;

        if(name == "texture_diffuse")
        {
            ss << diffuseNr++;
        }
        else if(name == "texture_specular")
        {
            ss << specularNr++;
        }
        number = ss.str(); 

        shader.set_float(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


#endif

