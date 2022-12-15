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

    void EntitiesRegistry::DestroyEntity(EntityHandle handle)
    {
        for(auto& [rtti, system] : m_componentSystems)
        {
            if(system->Has(handle))
            {
                system->Remove(handle);
            }
        }
    }

    template<class T>
    T *EntitiesRegistry::AddComponentTo(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Add(entityHandle);
    }

    template<class T>
    T *EntitiesRegistry::GetComponentOf(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Get(entityHandle);
    }

    template<class T>
    bool EntitiesRegistry::HasComponent(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Has(entityHandle);
    }

    template<class T>
    void EntitiesRegistry::RemoveComponentOf(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                .Remove(entityHandle);
    }

    template<class T>
    void EntitiesRegistry::View(ComponentSystem<T>::ViewCallback callback)
    {
        GetSystem<T>().View(callback);
    }

    // private

    template<class T>
    void EntitiesRegistry::AddSystem()
    {
        // TODO : pre instanciate Core component systems to be close in memory
        m_componentSystems.insert_or_assign(T::getClassRTTI(), std::make_unique<T>());
    }

    template<class T>
    void EntitiesRegistry::GetSystem()
    {
        if(!m_componentSystems.contains(T::getClassRTTI()))
            AddSystem<T>();

        return static_cast<T*>(m_componentSystems[T::getClassRTTI()].get());
    }

    void EntitiesRegistry::StartAll()
    {
        for(auto& [rtti, system] : m_componentSystems)
        {
            system->Start();
        }
    }

    void EntitiesRegistry::UpdateAllUpdatable(const float& deltaTime)
    {
        for(auto& [rtti, system] : m_componentSystems)
        {
            system->Update(deltaTime);
        }
    }

    void EntitiesRegistry::RenderAllRenderer(sf::RenderTarget& renderTarget)
    {
        for(auto& [rtti, system] : m_componentSystems)
        {
            system->Render(renderTarget);
        }
    }

} // Engine