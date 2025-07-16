#pragma once
#include <unordered_map>

namespace cabrankengine
{
	// Keeps track of the player inputs
	class InputManager
	{
	public:
		InputManager();

		// Update de key map.
		void update();

		// Sets the given key as pressed.
		void pressKey(unsigned int keyId);

		// Sets the given key as released.
		void releaseKey(unsigned int keyId);

		// True if the key is held down.
		bool isKeyDown(unsigned int keyId);

		// True if the key was just pressed.
		bool isKeyJustPressed(unsigned int keyId);

	private:
		// True if the key was just released.
		bool wasKeyDown(unsigned int keyId);

		std::unordered_map<unsigned int, bool> m_KeyMap;
		std::unordered_map<unsigned int, bool> m_PrevKeyMap;
	};
}
