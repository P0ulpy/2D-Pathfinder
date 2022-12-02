//
// Created by Flo on 25/11/2022.
//

#include "Component.hpp"

namespace Engine
{
    const ComponentHandle ComponentHandle::Null = ComponentHandle(0);

    Component::Component(ComponentHandle handle)
        : m_handle(handle)
    {}

    void Component::onStart() {}
    void Component::onUpdate(const float &deltaTime) {}
    void Component::onRender() {}
    void Component::onImGuiRender() {}
    void Component::onDestroy() {}
}
