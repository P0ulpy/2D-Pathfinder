//
// Created by Flo on 15/12/2022.
//

#ifndef PATHFINDER_ICOMPONENTSYSTEM_HPP
#define PATHFINDER_ICOMPONENTSYSTEM_HPP

#include "../Entity/Entity.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <functional>

namespace Engine
{
    class Component;

    class IComponentSystem
    {
    public:
        virtual Component* Add(EntityHandle entityHandle) = 0;
        virtual void Remove(EntityHandle entityHandle) = 0;
        virtual bool Has(EntityHandle entityHandle) = 0;
        virtual Component* Get(EntityHandle entityHandle) = 0;

        using ViewCallback = std::function<void(Component*)>;

        virtual void View(ViewCallback callback) = 0;

        virtual void Start() = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render(sf::RenderTarget& renderTarget);
    };
}

#endif //PATHFINDER_ICOMPONENTSYSTEM_HPP
