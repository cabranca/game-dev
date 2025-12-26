#include <pch.h>
#include "DefaultLibrary.h"

#include <Cabrankengine/Renderer/Buffer.h>

namespace cabrankengine::scene {

	using namespace rendering;

	void DefaultLibrary::init() {
		setupTexture(s_WhiteTexture, 0xffffffff);
        setupTexture(s_BlackTexture, 0xff000000);
        setupTexture(s_FlatNormalTexture, 0xffff8080);

		setupQuad();
        setupCube();

        s_ErrorShader = Shader::create("assets/shaders/Error.glsl");
	}

	Ref<rendering::Texture2D> DefaultLibrary::getWhiteTexture() {
		return s_WhiteTexture;
	}

	Ref<rendering::Texture2D> DefaultLibrary::getBlackTexture() {
		return s_BlackTexture;
	}

	Ref<rendering::Texture2D> DefaultLibrary::getFlatNormalTexture() {
		return s_FlatNormalTexture;
	}

	Ref<rendering::Shader> DefaultLibrary::getErrorShader() {
		return s_ErrorShader;
	}

	Ref<rendering::VertexArray> DefaultLibrary::getCube() {
		return s_CubeVA;
	}

	Ref<rendering::VertexArray> DefaultLibrary::getQuad() {
		return s_QuadVA;
	}
	
    void DefaultLibrary::setupTexture(Ref<Texture2D>& tex, uint32_t data) {
        tex = Texture2D::create(s_Specs);
		tex->setData(&data, sizeof(uint32_t));
    }
	
    void DefaultLibrary::setupQuad() {
        std::array<float, (3+2)*4> quadVertices = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

		std::array<uint32_t, 6> quadIndices = { 0, 1, 2, 2, 3, 0 };

		s_QuadVA = VertexArray::create();
		auto quadVB = VertexBuffer::create(quadVertices.data(), sizeof(quadVertices));
		quadVB->setLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });
		s_QuadVA->addVertexBuffer(quadVB);
		auto quadIB = IndexBuffer::create(quadIndices.data(), quadIndices.size());
		s_QuadVA->setIndexBuffer(quadIB);
    }
	
    void DefaultLibrary::setupCube() {
        std::array<float, (3+3+2)*24> cubeVertices = {
            // Front Face (+Z)
                  // Pos               // Normal            // UV
             -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f ,
              0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f ,
              0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f ,
             -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f ,

            // Right Face (+X)
              0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f ,
              0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f ,
              0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f ,
              0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f ,

            // Back Face (-Z)
              0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f ,
             -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f ,
             -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f ,
              0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f ,

            // Left Face (-X)
             -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f ,
             -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f ,
             -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f ,
             -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f ,

            // Top Face (+Y)
             -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f ,
              0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f ,
              0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f ,
             -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f ,

            // Bottom Face (-Y)
             -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f ,
              0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f ,
              0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f ,
             -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f 
        };

        // Indices (6 caras * 2 triangulos * 3 indices = 36 indices)
        std::array<uint32_t, 36> cubeIndices = {
            0,  1,  2,  2,  3,  0,  // Front
            4,  5,  6,  6,  7,  4,  // Right
            8,  9,  10, 10, 11, 8,  // Back
            12, 13, 14, 14, 15, 12, // Left
            16, 17, 18, 18, 19, 16, // Top
            20, 21, 22, 22, 23, 20  // Bottom
        };

		s_CubeVA = VertexArray::create();

		auto cubeVB = VertexBuffer::create(cubeVertices.data(), sizeof(cubeVertices));
		cubeVB->setLayout({ 
            { ShaderDataType::Float3, "a_Position" }, 
            { ShaderDataType::Float3, "a_Normal" }, 
            { ShaderDataType::Float2, "a_TexCoord" } 
        });

		s_CubeVA->addVertexBuffer(cubeVB);

		auto cubeIB = IndexBuffer::create(cubeIndices.data(), cubeIndices.size());
		s_CubeVA->setIndexBuffer(cubeIB);
	}
} // namespace cabrankengine::scene
