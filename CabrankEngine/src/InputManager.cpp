#include "InputManager.h"

using namespace cabrankengine;

InputManager::InputManager() : m_KeyMap(), m_PrevKeyMap() {
}

void InputManager::update()
{
	for (auto& [key, value] : m_KeyMap)
		m_PrevKeyMap[key] = value;
}

void InputManager::pressKey(unsigned int keyId)
{
	m_KeyMap[keyId] = true;
}

void InputManager::releaseKey(unsigned int keyId)
{
	m_KeyMap[keyId] = false;
}

bool InputManager::isKeyDown(unsigned int keyId)
{
	auto it = m_KeyMap.find(keyId);
	if (it != m_KeyMap.end())
		return it->second;
	return false;
}

bool InputManager::isKeyJustPressed(unsigned int keyId)
{
	return isKeyDown(keyId) && !wasKeyDown(keyId);
}

bool InputManager::wasKeyDown(unsigned int keyId)
{
	auto it = m_PrevKeyMap.find(keyId);
	if (it != m_PrevKeyMap.end())
		return it->second;
	return false;
}