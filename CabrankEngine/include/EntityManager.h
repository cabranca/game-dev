#pragma once
#include <memory>
#include <Entity.h>
#include <vector>
#include <unordered_map>

namespace cabrankengine
{
	using EntPtr = std::shared_ptr<Entity>; // For now the idea is that the entities are shared between m_Entities and m_EntitiesToAdd.
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
		const Entities& getEntities() const;

		// Return all entities with the given tag.
		const Entities& getEntities(const std::string& tag) const;

	private:
		Entities m_Entities; // All the current entities.
		std::unordered_map<std::string, Entities> m_TagToEntities; // All the current entities grouped by their tag.
		Entities m_EntitiesToAdd; // Entities to be added in the next Entity Manager Update.
		size_t m_TotalEntities; // Total amount of entities since the game started.
	};
}

