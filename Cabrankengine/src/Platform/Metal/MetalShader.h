#pragma once

#include <Cabrankengine/Math/Mat4.h>
#include <Cabrankengine/Renderer/Shader.h>

// Forward declarations de metal-cpp
namespace MTL { class RenderPipelineState; class Function; class Library; }

namespace cabrankengine::platform::metal {

	class MetalShader : public rendering::Shader {
		public:
			MetalShader(const std::string& filepath);
			MetalShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
			virtual ~MetalShader();

			// Binds the shader program for use in rendering.
			virtual void bind() const override;

			// Unbinds the shader program, stopping its use in rendering.
			virtual void unbind() const override;

			// Sets an integer value in the shader program.
			void setInt(const std::string& name, int value) override;

			// Sets a int array in the shader program.
			void setIntArray(const std::string& name, uint32_t count, int* values) override;

			// Sets a float value in the shader program.
			void setFloat(const std::string& name, float value) override;

			// Sets a 3 float vector in the shader program.
			void setFloat3(const std::string& name, const math::Vector3& vector) override;

			// Sets a 4 float vector in the shader program.
			void setFloat4(const std::string& name, const math::Vector4& vector) override;

			// Sets a 4x4 float matrix in the shader program.
		    void setMat4(const std::string& name, const math::Mat4& value) override;

			// Returns the name of the shader program given by the user.
			const std::string& getName() const override { return m_Name; }

			// Getter para que la API pueda usar el PSO
        	MTL::RenderPipelineState* getPipelineState() const { return m_PipelineState; }

		private:
			// Reads the shader source code from a file.
			std::string readFile(const std::string& filepath);

			// Helper para compilar y crear el PSO
        	void createPipeline(const std::string& source);
			
			std::string m_Name;
			
			MTL::Library* m_Library = nullptr;
			MTL::Function* m_VertexFunction = nullptr;
			MTL::Function* m_FragmentFunction = nullptr;
			MTL::RenderPipelineState* m_PipelineState = nullptr;
	};
}
