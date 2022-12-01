//
// Created by Flo on 25/11/2022.
//

#include "Entity.hpp"

namespace Engine
{
    Entity::Entity(EntityHandle handle, Scene *scene)
        : m_handle(handle)
        , m_Scene(scene)
    {}
} // Engine