// #include "TextRenderer.h"

// #include <ft2build.h>
// #include FT_FREETYPE_H
// #include <glad/glad.h>
// #include <glm/ext/matrix_clip_space.hpp>

// #include <Cabrankengine/Core/Logger.h>
// #include <Cabrankengine/Math/MatrixFactory.h>

// #include "Buffer.h"
// #include "RenderCommand.h"
// #include "Shader.h"
// #include "Texture.h"
// #include "VertexArray.h"

// #include <iostream>


// using namespace glm;

// namespace cabrankengine {


//     TextRenderer::TextRenderer(unsigned int width, unsigned int height) : m_ShaderLibrary(std::make_unique<ShaderLibrary>()) {
//         // load and configure shader
//         m_TextShader = m_ShaderLibrary->load("assets/shaders/Text.glsl");
//         m_TextShader->setMat4("projection", math::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.f, 1.f));
//         m_TextShader->setInt("text", 0);

//         // configure vertex array/vertex buffer for texture quads
//         m_VertexArray = VertexArray::create();

//         m_VertexBuffer = VertexBuffer::create(6 * 4 * 4);

//         m_VertexBuffer->setLayout({ { ShaderDataType::Float2, "pos" }, {ShaderDataType::Float2, "tex"} });

//         m_VertexArray->addVertexBuffer(m_VertexBuffer);

//         m_VertexBuffer->unbind();
//         m_VertexArray->unbind();
//     }

//     void TextRenderer::load(const std::string& font, unsigned int fontSize)
//     {
//         // first clear the previously loaded Characters
//         m_Characters.clear();
        
//         // then initialize and load the FreeType library
//         FT_Library ft;
//         if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
//             CE_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
        
//         // load font as face
//         FT_Face face;
//         if (FT_New_Face(ft, font.c_str(), 0, &face))
//             CE_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
        
//         // set size to load glyphs as
//         FT_Set_Pixel_Sizes(face, 0, fontSize);
        
//         // disable byte-alignment restriction
//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // TODO: Abstract away
        
//         GLint swizzleMask[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };

//         // then for the first 128 ASCII characters, pre-load/compile their characters and store them
//         for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
//         {
//             // load character glyph 
//             if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//             {
//                 CE_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
//                 continue;
//             }
            
//             auto charTexture = Texture2D::create(face);

//             // now store character for later use
//             Character character = {
//                 charTexture,
//                 ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                 ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                 static_cast<unsigned int>(face->glyph->advance.x)
//             };
//             m_Characters.emplace(c, character);
//         }
        
//         // destroy FreeType once we're finished
//         FT_Done_Face(face);
//         FT_Done_FreeType(ft);
//     }

//     void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const Vector3& color)
//     {
//          // activate corresponding render state	
//          m_TextShader->bind();
//          m_TextShader->setFloat3("textColor", color);
//          m_VertexArray->bind();

//          // iterate through all characters
//          for (char c : text)
//          {
//              Character ch = m_Characters.at(c);

//              float xpos = x + ch.Bearing.x * scale;
//              float ypos = y + (m_Characters['H'].Bearing.y - ch.Bearing.y) * scale;

//              float w = ch.Size.x * scale;
//              float h = ch.Size.y * scale;
             
//              // update VBO for each character
//              float vertices[6][4] = {
//                  { xpos,     ypos + h,   0.0f, 1.0f },
//                  { xpos + w, ypos,       1.0f, 0.0f },
//                  { xpos,     ypos,       0.0f, 0.0f },

//                  { xpos,     ypos + h,   0.0f, 1.0f },
//                  { xpos + w, ypos + h,   1.0f, 1.0f },
//                  { xpos + w, ypos,       1.0f, 0.0f }
//              };
            
//              // render glyph texture over quad
//              ch.Texture->bind();
            
//              // update content of VBO memory
//              m_VertexBuffer->bind();
//              m_VertexBuffer->setData(vertices, sizeof(vertices));
//              m_VertexBuffer->unbind();
            
//              // render quad
//              RenderCommand::draw(m_VertexArray);
            
//              // now advance cursors for next glyph
//              x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
//          }
//          m_VertexArray->unbind();
//     }
// }
