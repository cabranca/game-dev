#pragma once

#include "Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern cabrankengine::Application* cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
