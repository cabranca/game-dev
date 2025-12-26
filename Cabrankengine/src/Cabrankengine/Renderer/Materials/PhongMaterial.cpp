#include <pch.h>

#include "PhongMaterial.h"

namespace cabrankengine::rendering {

	PhongMaterial::PhongMaterial(const Ref<Shader>& shader) : Material(shader) {}

	void PhongMaterial::setDiffuseMap(const Ref<Texture2D>& texture) {
		m_DiffuseMap = texture;
	}

	void PhongMaterial::setSpecularMap(const Ref<Texture2D>& texture) {
		m_SpecularMap = texture;
	}

	void PhongMaterial::setShininess(float shininess) {
		m_Shininess = shininess;
	}

	// Esta es la función clave que llama el Renderer antes de dibujar
	void PhongMaterial::bind() const {
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
} // namespace cabrankengine::rendering