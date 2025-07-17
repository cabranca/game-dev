#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Cabrankengine::Application* Cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
