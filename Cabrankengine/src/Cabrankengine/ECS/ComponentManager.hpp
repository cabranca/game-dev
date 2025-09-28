#pragma once

#include <unordered_map>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/ECS/Common.h>

namespace cabrankengine {

	// I do not see why there is an interface for an only child
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
				size_t index = m_Size
				m_EntityToIndex[e] = index;
				m_IndexToEntity[index] = e;
				m_Components[index] = component;
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

	class ComponentManager {
		public:
			template<typename T>
			void registerComponent() {
				const char* typeName = typeid(T).name();
				CE_CORE_ASSERT(!m_ComponentTypes.contains(typeName), "Component added twice!");
				m_ComponentTypes[typeName] = m_NextComponentType++;
				m_ComponentArrays[typeName] = std::make_shared<ComponentArray<T>>();
			}

			template<typename T>
			uint8_t getComponentType() {
				const char* typeName = typeid(T).name();
				CE_CORE_ASSERT(m_ComponentTypes.contains(typeName), "Component not registered!");
				return m_ComponentTypes[typeName];
			}

			template<typename T>
			void addComponent(Entity e, T component) {
				getComponentArray<T>()->insert(e, component);
			}

			template<typename T>
			void removeComponent(Entity e) {
				getComponentArray<T>()->remove(e);
			}

			template<typename T>
			T& getComponent(Entity e) {
				return getComponentArray<T>()->get(e);
			}

			void entityDestroyed(Entity e) {
				for (auto const& pair : m_ComponentArrays)
					pair.second->entityDestroyed(e);
			}

		private:
			std::unordered_map<const char*, uint8_t> m_ComponentTypes{};
			std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
			uint8_t m_NextComponentType = 0;

			// Learn more about static_pointer_cast
			template<typename T>
			std::shared_ptr<ComponentArray<T>> getComponentArray() {
				const char* typeName = typeid(T).name();
				CE_CORE_ASSERT(m_ComponentTypes.contains(typeName), "Component not registered!");
				return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
			}
	};
}