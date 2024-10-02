#include <PowerUp.h>

using namespace breakout;

PowerUp::PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
    : GameObject(position, c_Size, texture, color, c_Velocity), m_Type(type), m_Duration(duration), m_Activated(false) {
}

