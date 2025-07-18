#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern cabrankengine::Application* cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	cabrankengine::Logger::init();
	CE_CORE_WARN("Initialized log!");
	CE_INFO("This is a client log :)");

	auto app = cabrankengine::CreateApplication();
	app->Run();
	delete app;
}

#endif
