#include <Entity.h>

using namespace cabrankengine;

void cabrankengine::Entity::destroy()
{
}

size_t cabrankengine::Entity::getId()
{
	return m_Id;
}

std::string cabrankengine::Entity::getTag()
{
	return m_Tag;
}

bool cabrankengine::Entity::isActive()
{
	return m_Active;
}
