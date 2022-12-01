//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../Entity/Entity.hpp"

namespace Engine
{
    /* Entities registry is the core storage of our ESC system
     * it manages Components allocation & association with an Entity
     * */
    class EntitiesRegistry
    {
    public:
        EntityHandle CreateEntity();
        void DestroyEntity(EntityHandle uuid);
    };

} // Engine

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
