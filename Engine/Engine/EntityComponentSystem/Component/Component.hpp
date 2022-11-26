//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

namespace Engine
{
    class Component
    {
    public:
        virtual void onStart();
        virtual void onUpdate(const float& deltaTime);
        virtual void onRender();
        virtual void onImGuiRender();
        virtual void onDestroy();
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
