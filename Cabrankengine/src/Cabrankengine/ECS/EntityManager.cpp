#include <pch.h>
#include <Cabrankengine/ECS/EntityManager.h>

#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine::ecs {

	EntityManager::EntityManager() {
		for (Entity e = 0; e < MAX_ENTITIES; e++)
			m_AvailableEntities.push(e);
	}

	Entity EntityManager::createEntity() {
		CE_CORE_ASSERT(m_LivingEntityCount < MAX_ENTITIES, "Maximum amount of entities reached!");
		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		m_LivingEntityCount++;
		return id;
	}

	void EntityManager::destroyEntity(Entity e) {
		CE_CORE_ASSERT(e < MAX_ENTITIES, "Entity out of bound Identifiers!");
		m_Signatures[e].reset();
		m_AvailableEntities.push(e);
		m_LivingEntityCount--;
	}

	void EntityManager::setSignature(Entity e, Signature signature) {
		CE_CORE_ASSERT(e < MAX_ENTITIES, "Entity out of bound Identifiers!");
		m_Signatures[e] = signature;
	}

	Signature EntityManager::getSignature(Entity e) {
		CE_CORE_ASSERT(e < MAX_ENTITIES, "Entity out of bound Identifiers!");
		return m_Signatures[e];
	}
}
