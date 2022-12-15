//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../Entity/Entity.hpp"
#include "../Component/Component.hpp"
#include "../../Core/Logger/Logger.hpp"
#include "ComponentSystem.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <unordered_map>
#include <map>
#include <stack>
#include <sstream>

namespace Engine
{
    /* Entities registry is the core interface for our ESC system
     * it manages Components association with Entities
     * */
    class EntitiesRegistry
    {
    private:
        std::unordered_map<RTTI*, std::unique_ptr<IComponentSystem>> m_componentSystems;

        template<class T = IComponentSystem>
        void AddSystem();

        template<class T = IComponentSystem>
        void GetSystem();

    public:

        EntityHandle CreateEntity();

        // TODO : Implement Entity Destruction and related Components pointers reset (explorer pointer ?)
        void DestroyEntity(EntityHandle handle);

        template<class T = Component>
        T* AddComponentTo(EntityHandle entityHandle);

        template<class T = Component>
        T* GetComponentOf(EntityHandle entityHandle);

        template<class T = Component>
        bool HasComponent(EntityHandle entityHandle);

        // TODO : found a way to notify Component destruction to all objects how reference it currently if a Component is removed accessing to his memory address don't throw any exception
        template<class T = Component>
        void RemoveComponentOf(EntityHandle entityHandle);

        template<class T = Component>
        void View(ComponentSystem<T>::ViewCallback callback);

        void StartAll();
        void UpdateAllUpdatable(const float& deltaTime);
        void RenderAllRenderer(sf::RenderTarget& renderTarget);
    };

} // Engine

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
