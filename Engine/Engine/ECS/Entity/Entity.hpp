//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_ENTITY_HPP
#define PATHFINDER_ENTITY_HPP

#include "../../Core/UUID.hpp"

namespace Engine
{
    class Scene;

    using EntityHandle = Core::UUID;

    class Entity
    {
    protected:
        Entity() = default;
        Entity(EntityHandle handle, Scene* scene);

    public:
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent();

        template<typename T>
        void RemoveComponent();

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

    private:
        EntityHandle m_handle;
        Scene* m_Scene = nullptr;

        friend class EntitiesRegistry;
        friend class Scene;
    };

} // Engine

#endif //PATHFINDER_ENTITY_HPP
