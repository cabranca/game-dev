#include <pch.h>
#include "PBRModelArch.h"

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/ECS/Registry.hpp>

namespace cbk::scene::arch {

	using namespace ecs;

	PBRModelArch::PBRModelArch() {
		auto reg = Application::get().getRegistry();
		m_Entity = reg->createEntity();
		reg->addComponent(m_Entity, CTransform());
		reg->addComponent(m_Entity, CPBRModel());
	}

	CTransform& PBRModelArch::transform() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CTransform>(m_Entity).value();
	}

	CPBRModel& PBRModelArch::model() {
		auto reg = Application::get().getRegistry();
		return *reg->getComponent<CPBRModel>(m_Entity).value();
	}
} // namespace cbk::scene::arch
