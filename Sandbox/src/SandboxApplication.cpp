#include <Cabrankengine.h>

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {
	}
	~Sandbox() {

	}
};

cabrankengine::Application* cabrankengine::CreateApplication() {
	return new Sandbox();
}
