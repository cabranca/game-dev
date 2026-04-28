#include <pch.h>
#include "BuiltInSystems.h"

#include <Cabrankengine/Renderer/Renderer2D.h>
#include <Cabrankengine/Renderer/TextRenderer.h>

#include "Components.h"

namespace cbk::ecs {

	using namespace rendering;
	using namespace scene;

	void CameraSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto camera = reg.getComponent<CCamera>(e);
			if (camera.value()->IsActive)
				m_ActiveCamera = camera.value()->Camera;
		}
	}

	const Ref<Camera>& CameraSystem::getActiveCamera() const {
		return m_ActiveCamera;
	}

	void SpriteRenderSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e);
			auto sprite = reg.getComponent<CSprite>(e);
			auto pos = transform.value()->Transform.Position;
			math::Vector3 size{ transform.value()->Transform.Scale };
			math::Vector3 rot{ transform.value()->Transform.Rotation };
			rendering::Renderer2D::drawRotatedQuad(pos, { size.x, size.y }, rot.z, sprite.value()->Texture, sprite.value()->TilingFactor,
			                                       sprite.value()->Tint);
		}
	}

	void PhongRenderSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e);
			auto model = reg.getComponent<CPhongModel>(e);
			model.value()->Model->draw(transform.value()->Transform.toMat4());
		}
	}

	void PBRRenderSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e);
			auto model = reg.getComponent<CPBRModel>(e);
			model.value()->Model->draw(transform.value()->Transform.toMat4());
		}
	}

	void TextRenderSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e).value();
			auto text = reg.getComponent<CText>(e).value();
			auto pos = transform->Transform.Position;
			math::Vector3 size{ transform->Transform.Scale };
			math::Vector3 rot{ transform->Transform.Rotation };
			TextRenderer::drawText(text->Text, transform->Transform.Position, text->FontScale, text->Color);
		}
	}
} // namespace cbk::ecs