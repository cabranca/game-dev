#include <pch.h>
#include "BuiltInSystems.h"

#include "Components.h"

namespace cbk::ecs {

    void CameraSystem::update(Registry& reg, float dt) {
        for (auto& e : m_Entities) {
                auto camera = reg.getComponent<CCamera>(e);
                if (camera.has_value() && camera.value()->IsActive)
                    m_ActiveCamera = camera.value()->Camera;
            }
    }
}