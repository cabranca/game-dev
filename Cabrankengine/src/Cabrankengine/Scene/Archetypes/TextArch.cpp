#include <pch.h>
#include "TextArch.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::scene::arch {

	using namespace ecs;

	TextArch::TextArch() {
		auto reg = Application::get().getRegistry();
		m_Entity = reg->createEntity();
		reg->addComponent(m_Entity, CTransform());
		reg->addComponent(m_Entity, CText());
	}

	CTransform& TextArch::transform() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CTransform>(m_Entity).value();
	}

	CText& TextArch::text() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CText>(m_Entity).value();
	}
} // namespace cbk::scene::arch