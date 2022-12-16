//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_SCENE_HPP
#define PATHFINDER_SCENE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "../Base.hpp"
#include "../../ECS/EntitiesRegistry/EntitiesRegistry.hpp"

namespace Engine
{
    class Entity;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        void OnStart();
        void OnUpdate(Timestep ts);
        void OnStop();

        void RenderScene(sf::RenderTarget& renderTarget);

        Entity CreateEntity();
        void DestroyEntity(Entity entity);

        Entity GetEntityByHandle(const EntityHandle& handle);

        inline EntitiesRegistry* GetEntitiesRegistry() { return &m_registry; }

    private:
        EntitiesRegistry m_registry;

        friend class Entity;
    };

} // Engine

#endif //PATHFINDER_SCENE_HPP
