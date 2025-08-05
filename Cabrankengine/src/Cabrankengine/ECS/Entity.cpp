#include <Cabrankengine/ECS/Entity.h>

namespace cabrankengine {
	Entity::Entity(const size_t& id) : m_Id(id), m_Active(true) {}

	void Entity::destroy()
	{
		m_Active = false;
	}

	size_t Entity::getId() const
	{
		return m_Id;
	}

	bool Entity::isActive() const
	{
		return m_Active;
	}
}
