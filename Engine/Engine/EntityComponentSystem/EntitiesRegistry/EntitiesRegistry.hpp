//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../../Core/UUID.hpp"

namespace Engine {

    /* Entities registry is the core storage of our ESC system
     * it manages Components allocation & association with an Entity
     * */
    class EntitiesRegistry
    {
    public:
        Core::UUID CreateEntity();
        void DestroyEntity(Core::UUID uuid);
    };

} // Engine

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
