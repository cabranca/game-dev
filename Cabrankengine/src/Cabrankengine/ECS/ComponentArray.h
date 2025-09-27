#pragma once

#include <unordered_map>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/ECS/Common.h>

namespace cabrankengine {

	class IComponentArray {
		public:
			virtual ~IComponentArray() = default;
			virtual void entityDestroyed(Entity e) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
		public:
			void insert(Entity e, T component) {
				CE_CORE_ASSERT(!m_EntityToIndex.contains(e), "The component has already been added!");
				m_EntityToIndex[e] = m_Size;
				m_IndexToEntity[m_Size] = e;
				m_Components[m_Size] = component;
				m_Size++;
			}

			void remove(Entity e) {
				CE_CORE_ASSERT(m_EntityToIndex.contains(e), "The component to remove does not exist!");
				size_t index = m_EntityToIndex[e];
				size_t lastIndex = m_Size - 1;
				m_Components[index] = components[lastIndex];
				Entity lastEntity = m_IndexToEntity[lastIndex];
				m_EntityToIndex[lastEntity] = index;
				m_IndexToEntity[index] = lastEntity;
				m_EntityToIndex.erase(e);
				m_IndexToEntity.erase(lastIndex);
				m_Size--;
			}

			T& get(Entity e) {
				CE_CORE_ASSERT(m_EntityToIndex.contains(e), "The component was not found!");
				return m_Components[m_EntityToIndex[e]];
			}

			void entityDestroyed(Entity e) override {
				if (m_EntityToIndex.contains(e))
					remove(e);
			}

		private:
			std::array<T, MAX_ENTITIES> m_Components{};
			std::unordered_map<Entity, size_t> m_EntityToIndex{};
			std::unordered_map<size_t, Entity> m_IndexToEntity{};
			size_t m_Size = 0;
	};
}