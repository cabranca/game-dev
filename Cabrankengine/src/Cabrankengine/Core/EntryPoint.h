#pragma once

// Despite being obvious, this is a good practice to be able to give a clear error message if the platform is not supported.
#if defined(CE_PLATFORM_WINDOWS) || defined(CE_PLATFORM_LINUX)

// The "extern" keyword is optional here but it is a good practice to use it to clearly show that the function is defined outside of this file.
// This expresion is similar to a forward declaration for functions.
extern cabrankengine::Application* cabrankengine::createApplication();

int main(int argc, char** argv) {
	cabrankengine::Logger::init();
	CE_CORE_INFO("Initialized logger!");

	auto app = cabrankengine::createApplication();
	app->Run();
	delete app;
}

#else
	#error "Cabrankengine only supports Windows and Linux!"
#endif
