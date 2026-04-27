#include <pch.h>
#include "PBRMaterial.h"

namespace cbk::rendering {

    using namespace scene;

	PBRMaterial::PBRMaterial() : Material(ShaderLibrary::get("PBR")), m_AlbedoColor(1.f), m_Metalness(0.f), m_Roughness(0.5f) {
        m_AlbedoMap = DefaultLibrary::getWhiteTexture();
        m_NormalMap = DefaultLibrary::getFlatNormalTexture();
        m_MetalRoughMap = DefaultLibrary::getWhiteTexture();
        m_AOMap = DefaultLibrary::getWhiteTexture();
    }

	void PBRMaterial::setAlbedoMap(const Ref<Texture2D>& tex) {
		m_AlbedoMap = tex;
	}

	void PBRMaterial::setNormalMap(const Ref<Texture2D>& tex) {
		m_NormalMap = tex;
	}

	void PBRMaterial::setMetalRoughMap(const Ref<Texture2D>& tex) {
		m_MetalRoughMap = tex;
	}

	void PBRMaterial::setAOMap(const Ref<Texture2D>& tex) {
		m_AOMap = tex;
	}

	math::Vector3 PBRMaterial::getAlbedoColor() const {
		return m_AlbedoColor;
	}

	float PBRMaterial::getMetalness() const {
		return m_Metalness;
	}

	float PBRMaterial::getRoughness() const {
		return m_Roughness;
	}

	void PBRMaterial::setAlbedoColor(math::Vector3 color) {
		m_AlbedoColor = color;
	}

	void PBRMaterial::setMetalness(float value) {
		m_Metalness = value;
	}

	void PBRMaterial::setRoughness(float value) {
		m_Roughness = value;
	}

	void PBRMaterial::bind() const {
        m_Shader->bind();

		m_Shader->setFloat3("u_AlbedoColor", m_AlbedoColor);
		m_Shader->setFloat("u_Metalness", m_Metalness);
		m_Shader->setFloat("u_Roughness", m_Roughness);

		// PBR Slots convention
		// Slot 0: Albedo
		m_AlbedoMap->bind(0);
		m_Shader->setInt("u_AlbedoMap", 0);

		// Slot 1: Normal
		m_NormalMap->bind(1);
		m_Shader->setInt("u_NormalMap", 1);

		// Slot 2: Metal/Roughness
		m_MetalRoughMap->bind(2);
		m_Shader->setInt("u_MetalRoughMap", 2);

		// Slot 3: AO
		m_AOMap->bind(3);
		m_Shader->setInt("u_AOMap", 3);
	}
} // namespace cbk::rendering