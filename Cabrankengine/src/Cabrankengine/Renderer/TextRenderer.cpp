#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include "Buffer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace cabrankengine::rendering {

	using namespace math;

	struct TextVertex {
		Vector3 position;
		Vector4 color;
		Vector2 texCoord;
		float texIndex;
	};

	struct TextRendererData {
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		Ref<VertexArray> textVertexArray;
		Ref<VertexBuffer> textVertexBuffer;
		Ref<Shader> textShader;

		uint32_t quadIndexCount = 0;
		TextVertex* textVertexBufferBase = nullptr;
		TextVertex* textVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 0;
	};

	static TextRendererData s_Data;

	void TextRenderer::init() {
		s_Data.textVertexArray = VertexArray::create();

		s_Data.textVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(TextVertex));
		s_Data.textVertexBuffer->setLayout({ { ShaderDataType::Float3, "pos" },
		                                     { ShaderDataType::Float4, "color" },
		                                     { ShaderDataType::Float2, "texCoord" },
		                                     { ShaderDataType::Float, "texIndex" } });
		s_Data.textVertexArray->addVertexBuffer(s_Data.textVertexBuffer);

		s_Data.textVertexBufferBase = new TextVertex[s_Data.maxVertices];
		uint32_t* quadIndices = new uint32_t[s_Data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> textIndexBuffer = IndexBuffer::create(quadIndices, s_Data.maxIndices);
		s_Data.textVertexArray->setIndexBuffer(textIndexBuffer);
		delete[] quadIndices;

		s_Data.textShader = Shader::create("assets/shaders/Text.glsl");
		s_Data.textShader->bind();

		int32_t samplers[s_Data.maxTextureSlots];
		for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++)
			samplers[i] = i;
		
		s_Data.textShader->setIntArray("u_Textures", s_Data.maxTextureSlots, samplers);

		loadFont("assets/fonts/ocraext.ttf", 20);
	}

	void TextRenderer::shutdown() {
		delete[] s_Data.textVertexBufferBase;
	}

	void TextRenderer::beginScene(const math::Mat4& viewProjection) {
		s_Data.textShader->bind();
		s_Data.textShader->setMat4("u_ViewProjection", viewProjection);
		startBatch();
	}

	void TextRenderer::endScene() {
		flush();
	}

	void TextRenderer::flush() {
		if (s_Data.quadIndexCount == 0) return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.textVertexBufferPtr - (uint8_t*)s_Data.textVertexBufferBase);
		s_Data.textVertexBuffer->setData(s_Data.textVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++)
			s_Data.textureSlots[i]->bind(i);

		RenderCommand::drawIndexed(s_Data.textVertexArray, s_Data.quadIndexCount);
	}

	void TextRenderer::drawText(const std::string& text, math::Vector3 position, float scale, math::Vector4 color) {
		float x = position.x;
		float y = position.y;

		for (char c: text) {
			if (s_Data.quadIndexCount >= s_Data.maxIndices)
				nextBatch();
			
			Character ch = s_Characters.at(c);

			float xpos = x + ch.bearing.x * scale;
			float ypos = y + (s_Characters['H'].bearing.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float textureIndex = -1.f;

			for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
				if (s_Data.textureSlots[i] == ch.texture) {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == -1.f) {
				if (s_Data.textureSlotIndex >= s_Data.maxTextureSlots)
					nextBatch();
				
					textureIndex = (float)s_Data.textureSlotIndex;
					s_Data.textureSlots[s_Data.textureSlotIndex] = ch.texture;
					s_Data.textureSlotIndex++;
			}

            s_Data.textVertexBufferPtr->position = { xpos, ypos + h, position.z };
            s_Data.textVertexBufferPtr->color = color;
            s_Data.textVertexBufferPtr->texCoord = { 0.0f, 0.0f };
            s_Data.textVertexBufferPtr->texIndex = textureIndex;
            s_Data.textVertexBufferPtr++;

            s_Data.textVertexBufferPtr->position = { xpos + w, ypos + h, position.z };
            s_Data.textVertexBufferPtr->color = color;
            s_Data.textVertexBufferPtr->texCoord = { 1.0f, 0.0f };
            s_Data.textVertexBufferPtr->texIndex = textureIndex;
            s_Data.textVertexBufferPtr++;

            s_Data.textVertexBufferPtr->position = { xpos + w, ypos, position.z };
            s_Data.textVertexBufferPtr->color = color;
            s_Data.textVertexBufferPtr->texCoord = { 1.0f, 1.0f };
            s_Data.textVertexBufferPtr->texIndex = textureIndex;
            s_Data.textVertexBufferPtr++;

            s_Data.textVertexBufferPtr->position = { xpos, ypos, position.z };
            s_Data.textVertexBufferPtr->color = color;
            s_Data.textVertexBufferPtr->texCoord = { 0.0f, 1.0f };
            s_Data.textVertexBufferPtr->texIndex = textureIndex;
            s_Data.textVertexBufferPtr++;

            s_Data.quadIndexCount += 6;

            x += (ch.advance >> 6) * scale;
		}
	}

	void TextRenderer::loadFont(const std::string& font, unsigned int fontSize) {
		s_Characters.clear();

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			CE_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

		FT_Face face;
		if (FT_New_Face(ft, font.c_str(), 0, &face))
			CE_CORE_ERROR("ERROR::FREETYPE: Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, fontSize);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // TODO: Abstract away

		for (GLubyte c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				CE_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}

			// A texture for each letter. Ideally we'd use a TextureAtlas
			auto charTexture = Texture2D::create(face);

			Character character = { charTexture, Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				                    Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				                    static_cast<unsigned int>(face->glyph->advance.x) };
			s_Characters.emplace(c, character);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
	
	void TextRenderer::startBatch() {
		s_Data.quadIndexCount = 0;
		s_Data.textVertexBufferPtr = s_Data.textVertexBufferBase;
		s_Data.textureSlotIndex = 0;
	}
	
	void TextRenderer::nextBatch() {
		flush();
		startBatch();
	}
} // namespace cabrankengine::rendering
