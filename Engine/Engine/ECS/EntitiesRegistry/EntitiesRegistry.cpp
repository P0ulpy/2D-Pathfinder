//
// Created by Flo on 26/11/2022.
//

#include "EntitiesRegistry.hpp"

namespace Engine
{
    EntityHandle EntitiesRegistry::CreateEntity()
    {
        return EntityHandle(Core::UUID::CreateNew());
    }

    template<typename T>
    T *EntitiesRegistry::AddComponentTo(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Add(entityHandle);
    }

    template<typename T>
    T *EntitiesRegistry::GetComponentOf(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Get(entityHandle);
    }

    template<typename T>
    bool EntitiesRegistry::HasComponent(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Has(entityHandle);
    }

    template<typename T>
    void EntitiesRegistry::RemoveComponentOf(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Remove(entityHandle);
    }

    template<typename T>
    void EntitiesRegistry::View(ComponentSystem<T>::ViewCallback callback)
    {
        GetSystem<T>().View(callback);
    }

} // Engine