#pragma once

#include "Core.h"

namespace cabrankengine {

	class CABRANKENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

	private:
		void Run();
	};

	// To be defined in client.
	Application* CreateApplication();
}