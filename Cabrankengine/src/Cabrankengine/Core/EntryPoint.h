#pragma once

// Despite being obvious, this is a good practice to be able to give a clear error message if the platform is not supported.
#if defined(CE_PLATFORM_WINDOWS) || defined(CE_PLATFORM_LINUX)

// The "extern" keyword is optional here but it is a good practice to use it to clearly show that the function is defined outside of this file.
// This expresion is similar to a forward declaration for functions.
extern cabrankengine::Application* cabrankengine::createApplication();

int main(int argc, char** argv) {
	cabrankengine::Logger::init();
	CE_CORE_INFO("Initialized logger!");
	CE_PROFILE_BEGIN_SESSION("Startup", "CabrankengineProfile-Startup.json");

	auto app = cabrankengine::createApplication();
	CE_PROFILE_END_SESSION();

	CE_PROFILE_BEGIN_SESSION("Runtime", "CabrankengineProfile-Runtime.json");
	app->Run();
	CE_PROFILE_END_SESSION();

	CE_PROFILE_BEGIN_SESSION("Shutdown", "CabrankengineProfile-Shutdown.json");
	delete app;
	CE_PROFILE_END_SESSION();
}

#else
	#error "Cabrankengine only supports Windows and Linux!"
#endif
