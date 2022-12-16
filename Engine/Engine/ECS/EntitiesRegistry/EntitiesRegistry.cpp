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