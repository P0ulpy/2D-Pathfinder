//
// Created by Flo on 25/11/2022.
//

#include "Component.hpp"

using namespace Engine;

Component::Component(ComponentHandle handle)
    : m_handle(handle)
{}

void Component::onStart() {}
void Component::onUpdate(const float &deltaTime) {}
void Component::onRender() {}
void Component::onImGuiRender() {}
void Component::onDestroy() {}
