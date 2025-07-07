#pragma once

#include <Shader.h>
#include <unordered_map>

namespace cabrankengine {
    /// Holds all state information relevant to a character as loaded using FreeType.
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };


    // A renderer class for rendering text displayed by a font loaded using the 
    // FreeType library. A single font is loaded, processed into a list of Character
    // items for later rendering.
    class TextRenderer
    {
    public:
        TextRenderer(unsigned int width, unsigned int height);

        // Pre-compiles a list of characters from the given font.
        void load(std::string_view font, unsigned int fontSize);

        // Renders a string of text using the precompiled list of characters.
        void renderText(std::string_view text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
    
    private:
        // Render state.
        unsigned int m_Vao, m_Vbo;

        // Holds a list of pre-compiled Characters.
        std::unordered_map<char, Character> m_Characters;

        // Shader used for text rendering.
        Shader m_TextShader;
    };
}

