//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_ENTITY_HPP
#define PATHFINDER_ENTITY_HPP

#include "../../Core/UUID.hpp"

namespace Engine
{
    using EntityHandle = Core::UUID;

    class Scene;

    class Entity
    {
    protected:
        Entity() = default;

    public:
        Entity(EntityHandle handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T>
        T* AddComponent();

        template<typename T>
        T* GetComponent();

        template<typename T>
        bool HasComponent();

        template<typename T>
        void RemoveComponent();

        inline Scene* GetScene() { return m_Scene; }

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

    private:
        EntityHandle m_handle = EntityHandle::Null;
        Scene* m_Scene = nullptr;

        template <class TComponent>
        class ComponentSystem;
        friend class EntitiesRegistry;
        friend class Scene;
        friend class Component;
    };

} // Engine

#endif //PATHFINDER_ENTITY_HPP
