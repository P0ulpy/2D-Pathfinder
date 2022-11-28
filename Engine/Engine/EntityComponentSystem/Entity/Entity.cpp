//
// Created by Flo on 25/11/2022.
//

#include "Entity.hpp"

namespace Engine
{
    Entity::Entity(Core::UUID handle, Scene *scene)
        : m_handle(handle)
        , m_Scene(scene)
    {}
} // Engine