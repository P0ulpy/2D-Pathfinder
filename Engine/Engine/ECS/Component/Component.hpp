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
        Component(ComponentHandle handle);
    public:
        virtual void onStart();
        virtual void onUpdate(const float& deltaTime);
        virtual void onRender();
        virtual void onImGuiRender();
        virtual void onDestroy();

    private:
        ComponentHandle m_handle;

        friend class EntitiesRegistry;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
