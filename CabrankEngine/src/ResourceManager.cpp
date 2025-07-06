#include <ResourceManager.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "stb_image.h"

using namespace cabrankengine;

// Instantiate static variables
std::unordered_map<std::string, Texture2D> ResourceManager::m_Textures;
std::unordered_map<std::string, Shader> ResourceManager::m_Shaders;


Shader ResourceManager::loadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile, const std::string& name)
{
    m_Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return m_Shaders[name];
}

Shader ResourceManager::getShader(const std::string& name)
{
    return m_Shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* file, bool alpha, const std::string& name)
{
    m_Textures[name] = loadTextureFromFile(file, alpha);
    return m_Textures[name];
}

Texture2D ResourceManager::getTexture(const std::string& name)
{
    return m_Textures[name];
}

void ResourceManager::clear()
{
    // (properly) delete all shaders	
    for (auto& iter : m_Shaders)
        glDeleteProgram(iter.second.getId());
    // (properly) delete all textures
    for (auto& iter : m_Textures)
        glDeleteTextures(1, iter.second.getId());
}

Shader ResourceManager::loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile)
{
    try {
        std::string vertexCode = ReadFileToString(vShaderFile);
        std::string fragmentCode = ReadFileToString(fShaderFile);
        std::string geometryCode;

        if (!gShaderFile.empty()) {
            geometryCode = ReadFileToString(gShaderFile);
        }

        Shader shader;
        shader.compile(vertexCode, fragmentCode, geometryCode);
        return shader;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR::SHADER: Failed to read shader files - " << e.what() << std::endl;
        return Shader();
    }
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.setInternalFormat(GL_RGBA);
        texture.setImageFormat(GL_RGBA);
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

std::string ResourceManager::ReadFileToString(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open the file: " + path);
    }
    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());
    file.close();
    return content;
}
