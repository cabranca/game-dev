#pragma once

#include <Cabrankengine/Core/Input.h>

namespace cabrankengine {

	class WindowsInput : public Input {
	protected:
		virtual bool isKeyPressedImpl(KeyCode keyCode) override;
		virtual bool isMouseButtonPressedImpl(MouseCode mouseCode) override;
		virtual std::pair<float, float> getMousePositionImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};
}