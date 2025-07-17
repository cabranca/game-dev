#include <Cabrankengine.h>

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};

cabrankengine::Application* CreateApplication() {
	return new Sandbox();
}
