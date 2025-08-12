#pragma once

// Despite being obvious, this is a good practice to be able to give a clear error message if the platform is not supported.
#if defined(CE_PLATFORM_WINDOWS) || defined(CE_PLATFORM_LINUX)

// TODO: understand the "extern" keyword. I think the idea is to make a "forward declaration" of the function
extern cabrankengine::Application* cabrankengine::CreateApplication();

int main(int argc, char** argv)
{
	cabrankengine::Logger::init();
	CE_CORE_INFO("Initialized log!");

	auto app = cabrankengine::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error "Cabrankengine only supports Windows and Linux!"
#endif
