#pragma once

#include <string>
#include <glm/glm.hpp>

namespace cabrankengine {
    // General purpose shader object. Compiles from file, generates
    // compile/link-time error messages and hosts several utility 
    // functions for easy management.
    class Shader
    {
    public:
        Shader();
        
        // Sets the current shader as active.
        Shader& use();
        
        // Compiles the shader from given source code.
        void compile(std::string_view vertexSource, std::string_view fragmentSource, std::string_view geometrySource = ""); // note: geometry source code is optional 
        
        // Utility functions.
        void setFloat(const char* name, float value, bool useShader = false);
        void setInteger(const char* name, int value, bool useShader = false);
        void setVector2f(const char* name, float x, float y, bool useShader = false);
        void setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
        void setVector3f(const char* name, float x, float y, float z, bool useShader = false);
        void setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
        void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

        // Getters
        unsigned int getId() const { return m_Id; }

    private:
        // Shader identifier.
        unsigned int m_Id;

        // Checks if compilation or linking failed and if so, print the error logs.
        void checkCompileErrors(unsigned int object, std::string type);
    };
}
