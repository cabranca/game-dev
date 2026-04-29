#include <pch.h>
#include "BuiltInSystems.h"

#include <Cabrankengine/Core/Input.h>
#include <Cabrankengine/Renderer/Renderer2D.h>
#include <Cabrankengine/Renderer/TextRenderer.h>

#include "Components.h"

namespace cbk::ecs {

	using namespace math;
	using namespace rendering;
	using namespace scene;

	void CameraSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e).value();
			auto camera = reg.getComponent<CCamera>(e).value();
			if (m_WindowResized) {
				camera->AspectRatio = m_AspectRatio;
				m_WindowResized = false;
			}
			if (camera->IsActive) {
				Mat4 projectionMatrix;
				float halfHeight = 0.f, halfWidth = 0.f;
				switch (camera->Type) {
				case ProjectionType::Orthographic:
					projectionMatrix = math::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, camera->Near, camera->Far);
					break;
				case ProjectionType::Perspective:
					projectionMatrix = math::perspective(camera->FovY, camera->AspectRatio, camera->Near, camera->Far);
					break;
				}
				auto viewMatrix = inverseAffine(transform->Transform.toMat4());
				m_ViewProjectionMatrix = viewMatrix * projectionMatrix;
				m_CameraPos = transform->Transform.Position;
			}
		}
	}

	const Mat4& CameraSystem::getViewProjectionMatrix() const {
		return m_ViewProjectionMatrix;
	}

	const Vector3& CameraSystem::getCameraWorldPosition() const {
		return m_CameraPos;
	}

	bool CameraSystem::onWindowResize(WindowResizeEvent& e) {
		m_AspectRatio = static_cast<float>(e.getWidth()) / e.getHeight();
		m_WindowResized = true;
		return true;
	}

	void CameraControllerSystem::update(Registry& reg, float dt) {
		for (auto& e: m_Entities) {
			auto transform = reg.getComponent<CTransform>(e).value();
			auto controller = reg.getComponent<CCameraController>(e).value();

			auto [mouseX, mouseY] = Input::getMousePosition();
			const bool captureMouse = Input::isMouseButtonPressed(Mouse::ButtonRight);

			if (captureMouse) {
				Input::setInputMode(true, true);

				if (controller->MouseCaptured) {
					const float deltaX = mouseX - controller->LastMouseX;
					const float deltaY = mouseY - controller->LastMouseY;

					controller->Yaw -= deltaX * controller->MouseSensitivity;
					controller->Pitch -= deltaY * controller->MouseSensitivity;
					controller->Pitch = std::clamp(controller->Pitch, -89.0f, 89.0f);
				}

				controller->MouseCaptured = true;
			} else {
				Input::setInputMode(false, false);
				controller->MouseCaptured = false;
			}

			controller->LastMouseX = mouseX;
			controller->LastMouseY = mouseY;

			const float yawRadians = radians(controller->Yaw);
			const float pitchRadians = radians(controller->Pitch);

			Vector3 forward{ -std::sin(yawRadians) * std::cos(pitchRadians), std::sin(pitchRadians),
				             -std::cos(yawRadians) * std::cos(pitchRadians) };
			forward.normalized();

			Vector3 flatForward{ forward.x, 0.f, forward.z };
			flatForward.normalized();

			if (flatForward.lengthSquared() == 0.f)
				flatForward = Vector3::Forward;

			const Vector3 flatRight = cross(flatForward, Vector3::Up).normalize();

			Vector3 movement{};

			if (Input::isKeyPressed(Key::W))
				movement += flatForward;
			if (Input::isKeyPressed(Key::S))
				movement -= flatForward;
			if (Input::isKeyPressed(Key::A))
				movement -= flatRight;
			if (Input::isKeyPressed(Key::D))
				movement += flatRight;
			if (Input::isKeyPressed(Key::Space))
				movement += Vector3::Up;
			if (Input::isKeyPressed(Key::LeftShift))
				movement += Vector3::Down;

			if (movement.lengthSquared() > 0.f) {
				movement.normalized();
				transform->Transform.Position += movement * controller->TranslationSpeed * dt;
			}

			transform->Transform.Rotation = { controller->Pitch, controller->Yaw, 0.f };
		}
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