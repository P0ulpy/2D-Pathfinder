//
// Created by Flo on 25/11/2022.
//

#include "../../../Engine/Core/Scene/Scene.hpp"
#include "../../../Engine/EntityComponentSystem/Entity/Entity.hpp"

namespace Engine {
    Entity Scene::CreateEntity(const Core::UUID &uuid)
    {
        Entity entity = { m_registry.CreateEntity(), this };
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_entityMap.erase(entity.GetHandle());
        m_registry.DestroyEntity(entity.GetHandle());
    }
} // Engine