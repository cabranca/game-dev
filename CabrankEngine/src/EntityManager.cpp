#include <EntityManager.h>

using namespace cabrankengine;

EntityManager::EntityManager() 
	: m_Entities(), m_TagToEntities(), m_EntitiesToAdd(), m_TotalEntities(0) {}

void EntityManager::update()
{
	// Remove dead entities
	std::erase_if(m_Entities, [](EntPtr ent) { return !ent->isActive(); });
	for (auto& [tag, entities] : m_TagToEntities)
	{
		std::erase_if(entities, [](EntPtr ent) { return !ent->isActive(); });
	}
	
	// Add new entities
	for (auto& ent : m_EntitiesToAdd)
	{
		m_Entities.push_back(ent);
		m_TagToEntities[ent->getTag()].push_back(ent); // TODO: check not existing key case.
	}
}

EntPtr EntityManager::addEntity(const std::string & tag)
{
	auto ent = EntPtr(new Entity(m_TotalEntities++, tag));
	m_EntitiesToAdd.push_back(ent);
	return ent;
}

Entities EntityManager::getEntities() const
{
	return m_Entities;
}

Entities EntityManager::getEntities(const std::string& tag) const
{
	return m_TagToEntities.at(tag);
}
