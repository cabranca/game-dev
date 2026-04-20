#pragma once

#include "Material.h"

namespace cbk::rendering {

	class PhongMaterial : public Material {
	  public:
		PhongMaterial(const Ref<Shader>& shader);

		void setDiffuseMap(const Ref<Texture2D>& texture);
		void setSpecularMap(const Ref<Texture2D>& texture);
		void setShininess(float shininess);

		void bind() const override;

	  private:
		Ref<Texture2D> m_DiffuseMap;
		Ref<Texture2D> m_SpecularMap;
		float m_Shininess = 32.0f; // Valor por defecto
	};
} // namespace cbk::rendering