#pragma once
#include <memory>
#include <Entity.h>
#include <vector>
#include <unordered_map>

namespace cabrankengine
{
	using EntPtr = std::shared_ptr<Entity>;

	class EntityManager
	{
	public:
		EntityManager();

		// Update the entities states.
		void update();

		// Create the entity and return it to the caller.
		EntPtr addEntity(const std::string& tag);

		// Return all entities.
		std::vector<EntPtr> getEntities() const;

		// Return all entities with the given tag.
		std::vector<EntPtr> getEntities(const std::string& tag) const;

	private:
		// Remove all the entities with m_Alive set to false.
		void removeDeadEntities(std::vector<EntPtr>& vec);

		std::vector<EntPtr> m_Entities; // All the current entities.
		std::unordered_map<std::string, std::vector<EntPtr>> m_TagToEntities; // All the current entities grouped by their tag.
		std::vector<EntPtr> m_EntitiesToAdd; // Entities to be added in the next Entity Manager Update.
		size_t m_TotalEntities; // Total amount of entities since the game started.
	};
}

