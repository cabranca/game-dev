#include <Cabrankengine/ECS/EntityManager.h>

namespace cabrankengine {

	EntityManager::EntityManager() : m_TotalEntities(0) {}

	void EntityManager::onUpdate()
	{
		// Remove dead entities
		std::erase_if(m_Entities, [](EntPtr ent) { return !ent->isActive(); });

		// Add new entities
		for (auto& ent : m_EntitiesToAdd)
		{
			m_Entities.push_back(ent);
		}
		m_EntitiesToAdd.clear();
	}

	EntPtr EntityManager::addEntity()
	{
		EntPtr ent = EntPtr(new Entity(m_TotalEntities++));
		m_EntitiesToAdd.push_back(ent);
		return ent;
	}

	const Entities& EntityManager::getEntities() const
	{
		return m_Entities;
	}
}
