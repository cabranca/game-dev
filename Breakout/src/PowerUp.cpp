#include <PowerUp.h>

PowerUp::PowerUp(std::string type, vec3 color, float duration, vec2 position, Texture2D texture)
    : GameObject(position, c_Size, texture, color, c_Velocity), m_Type(type), m_Duration(duration), m_Activated(false) {
}

