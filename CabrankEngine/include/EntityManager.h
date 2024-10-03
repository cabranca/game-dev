#pragma once
#include <memory>
#include <Entity.h>
#include <vector>
#include <unordered_map>

namespace cabrankengine
{
	using EntPtr = std::shared_ptr<Entity>;
	using Entities = std::vector<EntPtr>;

	class EntityManager
	{
	public:
		EntityManager();

		// Update the entities states.
		void update();

		// Create the entity and return it to the caller.
		EntPtr addEntity(const std::string& tag);

		// Return all entities.
		Entities getEntities() const;

		// Return all entities with the given tag.
		Entities getEntities(const std::string& tag) const;

	private:
		Entities m_Entities; // All the current entities.
		std::unordered_map<std::string, Entities> m_TagToEntities; // All the current entities grouped by their tag.
		Entities m_EntitiesToAdd; // Entities to be added in the next Entity Manager Update.
		size_t m_TotalEntities; // Total amount of entities since the game started.
	};
}

