#pragma once

#include <memory>
#include "Core.h"



namespace cabrankengine {
	class Window;
	class CABRANKENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

	// To be defined in client.
	Application* CreateApplication();
}