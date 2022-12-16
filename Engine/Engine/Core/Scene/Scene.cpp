//
// Created by Flo on 25/11/2022.
//

#include "Scene.hpp"

namespace Engine
{
    Entity Scene::CreateEntity()
    {
        Entity entity = { m_registry.CreateEntity(), this };
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.DestroyEntity(entity.GetHandle());
    }

    void Scene::OnStart() {

    }

    void Scene::OnUpdate(Timestep ts)
    {
        m_registry.UpdateAllUpdatable(ts);
    }

    void Scene::OnStop() {

    }

    Entity Scene::GetEntityByHandle(const EntityHandle &handle)
    {
        return Entity(handle, this);
    }

    void Scene::RenderScene(sf::RenderTarget &renderTarget)
    {
        m_registry.RenderAllRenderer(renderTarget);
    }
} // Engine