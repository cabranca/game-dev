#pragma once

#include <Cabrankengine/Math/Mat4.h>
#include <Cabrankengine/Renderer/Shader.h>

// TODO: Remove!!
typedef unsigned int GLenum;

namespace cabrankengine::platform::opengl {

	class OpenGLShader : public rendering::Shader {
		public:
			OpenGLShader(const std::string& filepath);
			OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
			virtual ~OpenGLShader();

			// Binds the shader program for use in rendering.
			virtual void bind() const override;

			// Unbinds the shader program, stopping its use in rendering.
			virtual void unbind() const override;

			// Sets an integer value in the shader program.
			void setInt(const std::string& name, int value) override;

			// Sets a int array in the shader program.
			void setIntArray(const std::string& name, uint32_t count, int* values);

			// Sets a float value in the shader program.
			void setFloat(const std::string& name, float value) override;

			// Sets a float value in the shader program.
			void setFloat2(const std::string& name, math::Vector2 value) override;

			// Sets a 3 float vector in the shader program.
			void setFloat3(const std::string& name, math::Vector3 vector) override;

			// Sets a 4 float vector in the shader program.
			void setFloat4(const std::string& name, const math::Vector4& vector) override;

			// Sets a 4x4 float matrix in the shader program.
		    void setMat4(const std::string& name, const math::Mat4& value) override;

			// Returns the name of the shader program given by the user.
			const std::string& getName() const override { return m_Name; }

		private:
			// Reads the shader source code from a file.
			std::string readFile(const std::string& filepath);
			
			// Pre-processes the shader source code to retrieve the individual shader programs (vertex, fragment, etc.).
			std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
			
			// Compiles the shader source code into a shader program.
			void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

			uint32_t m_RendererId; // OpenGL renderer ID for the shader program
			std::string m_Name; // Name of the shader
	};
}
