#include <Cabrankengine/Core/Logger.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>

#include FT_FREETYPE_H

#include <TextRenderer.h>
#include <ResourceManager.h>

using namespace cabrankengine;
using namespace glm;

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    // load and configure shader
    m_TextShader = m_ShaderLibrary.load("assets/shaders/Text.glsl");
    m_TextShader.setMat4("projection", ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
    m_TextShader.setInt("text", 0);
    
    // configure vertex array/vertex buffer for texture quads
    m_VertexArray = VertexArray::create();
    float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

    m_VertexBuffer = VertexBuffer::create(squareVertices, sizeof(squareVertices));

    m_VertexBuffer->setLayout({ { ShaderDataType::Float3, "pos" }, {ShaderDataType::Float2, "tex"} });

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    auto squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

    m_VertexArray->setIndexBuffer(squareIB);
    m_VertexArray.bind();
    m_VertexArray->addVertexBuffer(m_VertexBuffer);

    m_VertexBuffer.unbind();
    m_VertexArray.unbind();
}

void TextRenderer::load(const std::string& font, unsigned int fontSize)
{
    // first clear the previously loaded Characters
    this->m_Characters.clear();
    // then initialize and load the FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        CE_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        CE_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // TODO: Abstract away
    // then for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            CE_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        m_Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::renderText(std::string_view text, float x, float y, float scale, vec3 color)
{
    // // activate corresponding render state	
    // this->m_TextShader.use();
    // this->m_TextShader.setVector3f("textColor", color);
    // glActiveTexture(GL_TEXTURE0);
    // glBindVertexArray(this->m_Vao);

    // // iterate through all characters
    // std::string_view::const_iterator c;
    // for (c = text.begin(); c != text.end(); c++)
    // {
    //     Character ch = m_Characters[*c];

    //     float xpos = x + ch.Bearing.x * scale;
    //     float ypos = y + (this->m_Characters['H'].Bearing.y - ch.Bearing.y) * scale;

    //     float w = ch.Size.x * scale;
    //     float h = ch.Size.y * scale;
    //     // update VBO for each character
    //     float vertices[6][4] = {
    //         { xpos,     ypos + h,   0.0f, 1.0f },
    //         { xpos + w, ypos,       1.0f, 0.0f },
    //         { xpos,     ypos,       0.0f, 0.0f },

    //         { xpos,     ypos + h,   0.0f, 1.0f },
    //         { xpos + w, ypos + h,   1.0f, 1.0f },
    //         { xpos + w, ypos,       1.0f, 0.0f }
    //     };
    //     // render glyph texture over quad
    //     glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //     // update content of VBO memory
    //     glBindBuffer(GL_ARRAY_BUFFER, this->m_Vbo);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     // render quad
    //     glDrawArrays(GL_TRIANGLES, 0, 6);
    //     // now advance cursors for next glyph
    //     x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    // }
    // glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);
}