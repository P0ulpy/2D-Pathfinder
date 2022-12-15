//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

#include "../../Core/UUID.hpp"
#include "../../Core/RTTI/RTTI.hpp"

namespace Engine
{
    using ComponentHandle = Core::UUID;

    namespace ComponentID
    {
        using ID = uint64_t;
        constexpr ID Null = 0;
    }

    class Component : public IHasRTTI
    {
    public:
        DECLARE_RTTI(Component, NoRTTIAncestor)

    public:
        virtual void OnAwake();
        virtual void OnStart();
        virtual void OnUpdate(const float& deltaTime);
        virtual void OnImGuiRender();
        virtual void OnDestroy();

    protected:
        Component() = default;
        virtual ~Component() = default;

    private:
        ComponentHandle m_handle = ComponentHandle::Null;

        friend class EntitiesRegistry;

        template <class TComponent>
        friend class ComponentSystem;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
