/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cabrankengine {
    // General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
    class Shader
    {
    public:
        // constructor
        Shader();
        // sets the current shader as active
        Shader& use();
        // compiles the shader from given source code
        void    compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional 
        // utility functions
        void    setFloat(const char* name, float value, bool useShader = false);
        void    setInteger(const char* name, int value, bool useShader = false);
        void    setVector2f(const char* name, float x, float y, bool useShader = false);
        void    setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
        void    setVector3f(const char* name, float x, float y, float z, bool useShader = false);
        void    setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void    setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
        void    setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
        void    setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

        unsigned int getId() const { return m_Id; }

    private:
        // state
        unsigned int m_Id;
        // checks if compilation or linking failed and if so, print the error logs
        void    checkCompileErrors(unsigned int object, std::string type);
    };
}

