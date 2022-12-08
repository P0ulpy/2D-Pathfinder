//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

#include "../../Core/UUID.hpp"

namespace Engine
{
    using ComponentHandle = Core::UUID;

    class Component
    {
    protected:
        Component() = default;
        virtual ~Component() = default;

    public:
        virtual void OnAwake();
        virtual void OnStart();
        virtual void OnUpdate(const float& deltaTime);
        virtual void OnRender();
        virtual void OnImGuiRender();
        virtual void OnDestroy();

    private:
        ComponentHandle m_handle = ComponentHandle::Null;

        friend class EntitiesRegistry;

        template <typename TComponent>
        friend class ComponentSystem;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
