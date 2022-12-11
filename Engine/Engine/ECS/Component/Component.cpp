//
// Created by Flo on 25/11/2022.
//

#include "Component.hpp"

namespace Engine
{
    const ComponentHandle ComponentHandle::Null = ComponentHandle(0);

    void Component::OnAwake() {}
    void Component::OnStart() {}
    void Component::OnUpdate(const float &deltaTime) {}
    void Component::OnRender() {}
    void Component::OnImGuiRender() {}
    void Component::OnDestroy() {}
}
