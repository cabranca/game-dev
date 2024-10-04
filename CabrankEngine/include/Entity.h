#pragma once
#include <string>
#include <Components.h>

namespace cabrankengine
{
	// This MUST match all the possible components in Components.h
	typedef std::tuple<
		CTransform,
		CTexture,
		CBoundingBox,
		CInput,
		CScore
	> ComponentTuple;

	class Entity
	{
	public:
		// Adds the given component
		template<typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{
			auto& component = getComponent<T>();
			component = T(std::forward<TArgs>(mArgs)...);
			component.has = true;
			return component;
		}

		// Sets m_Active to false
		void destroy();

		// Getters
		size_t getId() const;
		std::string getTag() const;
		bool isActive() const;

		template<typename T>
		bool hasComponent() const
		{
			return getComponent<T>.has;
		}

		template<typename T>
		T& getComponent()
		{
			return std::get<T>(m_Components);
		}

		template<typename T>
		const T& getComponent() const
		{
			return std::get<T>(m_Components);
		}
		
		template<typename T>
		void removeComponent()
		{
			getComponent<T>() = T();
		}

	private:
		friend class EntityManager;

		Entity(const size_t& id, const std::string& tag);

		size_t m_Id;
		std::string m_Tag;
		bool m_Active;

		ComponentTuple m_Components;
	};
}
