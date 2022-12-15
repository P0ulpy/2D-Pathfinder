//
// Created by Flo on 25/11/2022.
//

#include "Entity.hpp"
#include "../../Core/Scene/Scene.hpp"

namespace Engine
{
    Entity::Entity(EntityHandle handle, Scene *scene)
        : m_handle(handle)
        , m_Scene(scene)
    {}

    template<typename T>
    T* Entity::AddComponent()
    {
        return m_Scene->m_registry.template AddComponentTo<T>(m_handle);
    }

    template<typename T>
    T* Entity::GetComponent()
    {
        return m_Scene->m_registry.template GetComponentOf<T>(m_handle);
    }

    template<typename T>
    bool Entity::HasComponent()
    {
        return m_Scene->m_registry.template HasComponent<T>(m_handle);
    }

    template<typename T>
    void Entity::RemoveComponent()
    {
        m_Scene->m_registry.template RemoveComponentOf<T>(m_handle);
    }
} // Engine