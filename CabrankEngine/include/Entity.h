#pragma once
#include <string>

namespace cabrankengine
{
	class Entity
	{
	public:
		// Sets m_Active to false
		void destroy();

		// Getters
		size_t getId();
		std::string getTag();
		bool isActive();

	private:
		size_t m_Id;
		std::string m_Tag;
		bool m_Active;
	};
}
