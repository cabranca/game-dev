#include <Cabrankengine.h>

class Sandbox : public cabrankengine::Application {
public:
	Sandbox() {
		pushOverlay(new cabrankengine::ImGuiLayer());
	}
	~Sandbox() {

	}
};

cabrankengine::Application* cabrankengine::CreateApplication() {
	return new Sandbox();
}
