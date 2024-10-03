#include <EntityManager.h>

using namespace cabrankengine;

EntityManager::EntityManager() 
	: m_Entities(), m_TagToEntities(), m_EntitiesToAdd(), m_TotalEntities(0) {}

void EntityManager::update()
{
	for (auto& ent : m_EntitiesToAdd)
	{
		m_Entities.push_back(ent);
		m_TagToEntities[ent->getTag()].push_back(ent); // TODO: check not existing key case.
	}
}

EntPtr EntityManager::addEntity(const std::string & tag)
{
	auto ent = std::shared_ptr<Entity>(new Entity(m_TotalEntities++, tag));
	m_EntitiesToAdd.push_back(ent);
	return ent;
}

std::vector<EntPtr> EntityManager::getEntities() const
{
	return m_Entities;
}

std::vector<EntPtr> EntityManager::getEntities(const std::string& tag) const
{
	return m_TagToEntities.at(tag);
}

void EntityManager::removeDeadEntities(std::vector<EntPtr>& vec)
{

}
