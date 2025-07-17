#pragma once

#ifdef CE_PLATFORM_WINDOWS

#include "Application.h"

extern cabrankengine::Application* cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = cabrankengine::CreateApplication();
	app->Run();
	delete app;
}

#endif
