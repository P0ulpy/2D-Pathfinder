//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_ENTITY_HPP
#define PATHFINDER_ENTITY_HPP

#include "../../Core/UUID.hpp"

namespace Engine {

    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(Core::UUID handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent();

        template<typename T>
        void RemoveComponent();

        [[nodiscard]] inline Core::UUID GetHandle() const { return m_handle; }

    private:
        Core::UUID m_handle;
        Scene* m_Scene = nullptr;
    };

} // Engine

#endif //PATHFINDER_ENTITY_HPP
