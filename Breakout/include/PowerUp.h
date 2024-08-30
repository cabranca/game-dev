#pragma once
#include <GameObject.h>

namespace breakout {
    const glm::vec2 c_Size = glm::vec2(60.0f, 20.0f);
    const glm::vec2 c_Velocity = glm::vec2(0.0f, 150.0f);

    class PowerUp : public GameObject
    {
    public:
        PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture);

        const std::string& GetType() const { return m_Type; }
        float GetDuration() const { return m_Duration; }
        void DecreaseDuration(float delta) { m_Duration -= delta; }
        bool IsActivated() const { return m_Activated; }
        void SetActivated(bool value) { m_Activated = value; }

    private:
        std::string m_Type;
        float m_Duration;
        bool m_Activated;
    };
}
