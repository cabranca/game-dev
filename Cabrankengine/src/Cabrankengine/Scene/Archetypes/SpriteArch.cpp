#include <pch.h>
#include "SpriteArch.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::scene::arch {

	using namespace ecs;

	SpriteArch::SpriteArch() {
		auto reg = Application::get().getRegistry();
		m_Entity = reg->createEntity();
		reg->addComponent(m_Entity, CTransform());
		reg->addComponent(m_Entity, CSprite());
	}

	CTransform& SpriteArch::transform() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CTransform>(m_Entity).value();
	}

	CSprite& SpriteArch::sprite() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CSprite>(m_Entity).value();
	}
} // namespace cbk::scene::arch