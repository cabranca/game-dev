#pragma once

#include "Material.h"

namespace cbk::rendering {

	class PBRMaterial : public Material {
	  public:
		PBRMaterial(const Ref<Shader>& shader);

		void setAlbedoMap(const Ref<Texture2D>& tex);
		void setNormalMap(const Ref<Texture2D>& tex);
		void setMetalRoughMap(const Ref<Texture2D>& tex);
		void setAOMap(const Ref<Texture2D>& tex);

		math::Vector3 getAlbedoColor() const;
		float getMetalness() const;
		float getRoughness() const;
		
		void setAlbedoColor(math::Vector3 color);
		void setMetalness(float value);
		void setRoughness(float value);

        void bind() const override;

	  private:
		Ref<Texture2D> m_AlbedoMap;
		Ref<Texture2D> m_NormalMap;
		Ref<Texture2D> m_MetalRoughMap;
		Ref<Texture2D> m_AOMap;

		math::Vector3 m_AlbedoColor;
		float m_Metalness;
		float m_Roughness;
	};
} // namespace cbk::rendering