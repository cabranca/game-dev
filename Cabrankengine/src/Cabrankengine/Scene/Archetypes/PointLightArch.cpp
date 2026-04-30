#include <pch.h>
#include "PointLightArch.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::scene::arch {

	using namespace ecs;

	PointLightArch::PointLightArch() {
		auto reg = Application::get().getRegistry();
		m_Entity = reg->createEntity();
		reg->addComponent(m_Entity, CTransform());
		reg->addComponent(m_Entity, CPointLight());
	}

	CTransform& PointLightArch::transform() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CTransform>(m_Entity).value();
	}

	CPointLight& PointLightArch::light() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CPointLight>(m_Entity).value();
	}
} // namespace cbk::scene::arch
