#pragma once

#include <array>
#include <bitset>
#include <queue>

namespace cabrankengine {

	using Entity = uint32_t;
	constexpr Entity MAX_ENTITIES = 5000;
	const size_t MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class EntityManager {
		public:
			EntityManager();

			Entity createEntity();

			void destroyEntity(Entity e);

			void setSignature(Entity e, Signature signature);

			Signature getSignature(Entity e);

		private:
			std::queue<Entity> m_AvailableEntities{};
			std::array<Signature, MAX_ENTITIES> m_Signatures{};
			uint32_t m_LivingEntityCount = 0;
	};
}

