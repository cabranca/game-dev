#pragma once

#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/Texture.h>
#include <Cabrankengine/Scene/DefaultLibrary.h>


namespace cbk::rendering {

	class Material {
	  public:
		Material(const Ref<Shader>& shader) : m_Shader(shader) {}
		virtual ~Material() = default;

		virtual void bind() const = 0;

		Ref<Shader> getShader() const {
			return m_Shader;
		}

	  protected:
		Ref<Shader> m_Shader;
	};

} // namespace cbk::rendering