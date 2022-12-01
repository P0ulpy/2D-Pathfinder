//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_SCENE_HPP
#define PATHFINDER_SCENE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "../Base.hpp"
#include "../../ECS/EntitiesRegistry/EntitiesRegistry.hpp"
#include "../../ECS/Entity/Entity.hpp"

namespace Engine
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void OnStart();
        void OnUpdate(Timestep ts);
        void OnStop();

        void RenderScene(sf::RenderTarget& renderTarget);

        Entity CreateEntity(const EntityHandle& entityHandle);
        void DestroyEntity(Entity entity);

        Entity GetEntityByUUID(const Core::UUID& uuid);

    private:
        EntitiesRegistry m_registry;

        friend class Entity;
    };

} // Engine

#endif //PATHFINDER_SCENE_HPP
