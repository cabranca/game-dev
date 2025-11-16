// #pragma once

// #include <string>
// #include <unordered_map>

// #include <glm/glm.hpp>

// #include <Cabrankengine/Core/Core.h>
// #include <Cabrankengine/Math/MatrixFactory.h>

// namespace cabrankengine {

//     // Forward declarations
//     class Shader;
//     class ShaderLibrary;
//     class Texture2D;
//     class VertexArray;
//     class VertexBuffer;

//     /// Holds all state information relevant to a character as loaded using FreeType.
//     struct Character {
//         Ref<Texture2D> Texture; // ID handle of the glyph texture
//         glm::ivec2   Size;      // Size of glyph
//         glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
//         unsigned int Advance;   // Horizontal offset to advance to next glyph
//     };


//     // A renderer class for rendering text displayed by a font loaded using the 
//     // FreeType library. A single font is loaded, processed into a list of Character
//     // items for later rendering.
//     class TextRenderer
//     {
//     public:
//         TextRenderer(unsigned int width, unsigned int height);

//         // Pre-compiles a list of characters from the given font.
//         void load(const std::string& font, unsigned int fontSize);

//         // Renders a string of text using the precompiled list of characters.
//         void renderText(const std::string& text, float x, float y, float scale, const math::Vector3& color = math::Vector3(1.f));
    
//     private:
//         // Render state.
//         Ref<VertexBuffer> m_VertexBuffer; // Replaces older unsigned int m_Vao
//         Ref<VertexArray> m_VertexArray; // Replaces older unsigned int m_Vbo

//         // Holds a list of pre-compiled Characters.
//         std::unordered_map<char, Character> m_Characters;

//         Scope<ShaderLibrary> m_ShaderLibrary;
//         // Shader used for text rendering.
//         Ref<Shader> m_TextShader;
//     };
// }

