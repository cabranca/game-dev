/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once


namespace cabrankengine {
    // Texture2D is able to store and configure a texture in OpenGL.
    // It also hosts utility functions for easy management.
    class Texture2D
    {
    public:
        Texture2D();
        // generates texture from image data
        void generate(unsigned int width, unsigned int height, unsigned char* data);
        // binds the texture as the current active GL_TEXTURE_2D texture object
        void bind() const;

        unsigned int* getId() { return &m_Id; }
        void setInternalFormat(unsigned int value) { m_InternalFormat = value; }
        void setImageFormat(unsigned int value) { m_ImageFormat = value; }
    
    private:
        // holds the ID of the texture object, used for all texture operations to reference to this particular texture
        unsigned int m_Id;
        // texture image dimensions
        unsigned int m_Width, m_Height; // width and height of loaded image in pixels

        // texture Format
        unsigned int m_InternalFormat; // format of texture object
        unsigned int m_ImageFormat; // format of loaded image

        // texture configuration
        unsigned int m_WrapS; // wrapping mode on S axis
        unsigned int m_WrapT; // wrapping mode on T axis
        unsigned int m_FilterMin; // filtering mode if texture pixels < screen pixels
        unsigned int m_FilterMax; // filtering mode if texture pixels > screen pixels
    };
}

