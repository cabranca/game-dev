#pragma once

#include "Shader.h"
#include "Texture.h"

namespace cabrankengine::rendering {

	class Material {
	  public:
		Material(const Ref<Shader>& shader) : m_Shader(shader) {}

		virtual ~Material() = default;

		void setDiffuseMap(const Ref<Texture2D>& texture) {
			m_DiffuseMap = texture;
		}
		void setSpecularMap(const Ref<Texture2D>& texture) {
			m_SpecularMap = texture;
		}
		void setShininess(float shininess) {
			m_Shininess = shininess;
		}

		Ref<Shader> getShader() const {
			return m_Shader;
		}

		// Esta es la función clave que llama el Renderer antes de dibujar
		void bind() const {
			m_Shader->bind();
			
			// 1. Configurar propiedades escalares
			// Nota: "material.shininess" debe coincidir con tu struct en Lightning.glsl
			m_Shader->setFloat("material.shininess", m_Shininess);

			// 2. Configurar Texturas (Slots fijos por convención)
			// Slot 0 = Diffuse
			if (m_DiffuseMap) {
				m_DiffuseMap->bind(0);
				m_Shader->setInt("material.diffuse", 0);
			}

			// Slot 1 = Specular
			if (m_SpecularMap) {
				m_SpecularMap->bind(1);
				m_Shader->setInt("material.specular", 1);
			}
		}

		static Ref<Material> create(const Ref<Shader>& shader) {
			return std::make_shared<Material>(shader);
		}

	  private:
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_DiffuseMap;
		Ref<Texture2D> m_SpecularMap;
		float m_Shininess = 32.0f; // Valor por defecto
	};
} // namespace cabrankengine::rendering