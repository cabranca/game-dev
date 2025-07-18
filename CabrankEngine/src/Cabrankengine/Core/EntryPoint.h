#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern cabrankengine::Application* cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	cabrankengine::Logger::init();
	auto app = cabrankengine::CreateApplication();
	app->Run();
	delete app;
}

#endif
