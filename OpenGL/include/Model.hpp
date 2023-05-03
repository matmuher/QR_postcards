#ifndef MODEL_H
#define MODEL_H



#include "Mesh.hpp"

/**
 * @brief TextureFromFile служит для загрузки текстуры из файла
 * 
 * @return идентификтор загруженной текстуры
 */
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);


//---------------------------------------------------Model----------------------------------------------------------


/**
 * @brief class Model служит для представления объектного файла модели в программе
 */
class Model 
{
    public:

        /**
         * @param path - путь к объектному файлу модели
         */
        Model(const char *path)
        {
            loadModel(path);
        }
        Model() {}

        /**
         * @brief draw - функция отрисовки модели на экране
         * 
         * @param shader - шейдерная программа, по которой производится отрисовка
         */
        void draw(Shader shader);	
    private:
        
        /// Сетки, составляющий данную модель
        std::vector<Mesh> meshes_;

        /// Директория с объектным файлом данной модели
        std::string directory_;

        /// Текстуры сеток
        std::vector<Texture_Mesh> textures_loaded_;

        /**
         * @brief loadModel - функция для обработки объектного файла и создания модели
         * 
         * @param path - путь к объектному файлу модели
         */
        void loadModel(std::string path);

        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture_Mesh> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


//-------------------------------------------------------------------------------------------------------------------


void Model::draw(Shader shader)
{
    for (int i = 0; i < meshes_.size(); i++)
    {
        meshes_[i].draw(shader);
    }
}


void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);    
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory_ = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes_.push_back(processMesh(mesh, scene));         
    }
    
    for (int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}  


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture_Mesh> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f); 
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    } 
    
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture_Mesh> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture_Mesh> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}  


std::vector<Texture_Mesh> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture_Mesh> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded_.size(); j++)
        {
            if(std::strcmp(textures_loaded_[j].path.C_Str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded_[j]);
                skip = true; 
                break;
            }
        }

        if (!skip)
        {
            Texture_Mesh texture;
            texture.id = TextureFromFile(str.C_Str(), directory_);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);

            textures_loaded_.push_back(texture);
        }
    }

    return textures;
}


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = path;
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


#endif

