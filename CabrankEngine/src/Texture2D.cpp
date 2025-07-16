/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>
#include <GL/glew.h>
#include <Texture2D.h>

using namespace cabrankengine;

Texture2D::Texture2D()
    : m_Width(0), m_Height(0), m_InternalFormat(GL_RGB), m_ImageFormat(GL_RGB), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT), m_FilterMin(GL_LINEAR), m_FilterMax(GL_LINEAR)
{
    glGenTextures(1, &this->m_Id);
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->m_Width = width;
    this->m_Height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->m_InternalFormat, width, height, 0, this->m_ImageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_FilterMax);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->m_Id);
}