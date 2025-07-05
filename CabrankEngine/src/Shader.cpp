/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <Shader.h>
#include <GL/glew.h>
#include <iostream>

using namespace cabrankengine;

Shader::Shader() : m_Id(0)
{
}

Shader& Shader::use()
{
    glUseProgram(m_Id);
    return *this;
}

void Shader::compile(std::string_view vertexSource, std::string_view fragmentSource, std::string_view geometrySource)
{
    auto compileShader = [this](GLenum type, std::string_view source, const char* typeName) -> GLuint {
        GLuint shader = glCreateShader(type);
        const char* src = source.data();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, typeName);
        return shader;
    };

    // 1. Compilar shaders
    GLuint sVertex = compileShader(GL_VERTEX_SHADER, vertexSource, "VERTEX");
    GLuint sFragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT");

    GLuint gShader = 0;
    if (!geometrySource.empty()) {
        gShader = compileShader(GL_GEOMETRY_SHADER, geometrySource, "GEOMETRY");
    }

    // shader program
    this->m_Id = glCreateProgram();
    glAttachShader(this->m_Id, sVertex);
    glAttachShader(this->m_Id, sFragment);
    if (gShader) 
        glAttachShader(this->m_Id, gShader);
    glLinkProgram(this->m_Id);
    checkCompileErrors(this->m_Id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (gShader)
        glDeleteShader(gShader);
}

void Shader::setFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->m_Id, name), value);
}
void Shader::setInteger(const char* name, int value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->m_Id, name), value);
}
void Shader::setVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->m_Id, name), x, y);
}
void Shader::setVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->m_Id, name), value.x, value.y);
}
void Shader::setVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->m_Id, name), x, y, z);
}
void Shader::setVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->m_Id, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->m_Id, name), x, y, z, w);
}
void Shader::setVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->m_Id, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->m_Id, name), 1, false, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}