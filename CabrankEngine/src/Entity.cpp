#include <Entity.h>

using namespace cabrankengine;

Entity::Entity(const size_t& id, const std::string& tag) 
	: m_Id(id), m_Tag(tag), m_Active(true), m_Components() {}

void Entity::destroy()
{
	m_Active = false;
}

size_t Entity::getId() const
{
	return m_Id;
}

std::string Entity::getTag() const
{
	return m_Tag;
}

bool Entity::isActive() const
{
	return m_Active;
}
