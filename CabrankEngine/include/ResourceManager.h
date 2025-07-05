#pragma once

#include <unordered_map>
#include <string>
#include <texture2D.h>
#include <shader.h>

namespace cabrankengine {
    // A static singleton ResourceManager class that hosts several
    // functions to load Textures and Shaders. Each loaded texture
    // and/or shader is also stored for future reference by string
    // handles. All functions and resources are static and no 
    // public constructor is defined.
    class ResourceManager
    {
    public:
        ResourceManager() = delete;

        // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
        static Shader loadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile, const std::string& name);
        
        // retrieves a stored sader
        static Shader getShader(const std::string& name);
        
        // loads (and generates) a texture from file
        static Texture2D loadTexture(const char* file, bool alpha, const std::string& name);
        
        // retrieves a stored texture
        static Texture2D getTexture(const std::string& name);
        
        // properly de-allocates all loaded resources
        static void clear();

    private:
        // resource storage
        static std::unordered_map<std::string, Shader> m_Shaders;
        static std::unordered_map<std::string, Texture2D> m_Textures;

        // loads and generates a shader from file
        static Shader    loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile = nullptr);
        // loads a single texture from file
        static Texture2D loadTextureFromFile(const char* file, bool alpha);

        static std::string ReadFileToString(const std::string& path);
    };

}
