//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

#include "../../Core/UUID.hpp"
#include "../../Core/RTTI/RTTI.hpp"
#include "../Entity/Entity.hpp"

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

        /* Implementable Component Events methods
        void OnAwake();
        void OnStart();
        void OnUpdate(const float& deltaTime);
        void OnRender(sf::RenderTarget &renderTarget);
        void OnImGuiRender();
        void OnDestroy();
        */

        [[nodiscard]] Entity GetEntity() const { return m_entity; }

    protected:
        Component() = default;
        virtual ~Component() = default;

    private:
        ComponentHandle m_handle = ComponentHandle::Null;
        Entity m_entity { EntityHandle::Null, nullptr };

        friend class EntitiesRegistry;

        template <class TComponent>
        friend class ComponentSystem;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
