#include <pch.h>
#include "PhongModelArch.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::scene::arch {

	using namespace ecs;

	PhongModelArch::PhongModelArch() {
		auto reg = Application::get().getRegistry();
		m_Entity = reg->createEntity();
		reg->addComponent(m_Entity, CTransform());
		reg->addComponent(m_Entity, CPhongModel());
	}

	CTransform& PhongModelArch::transform() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CTransform>(m_Entity).value();
	}

	CPhongModel& PhongModelArch::model() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CPhongModel>(m_Entity).value();
	}
} // namespace cbk::scene::arch
