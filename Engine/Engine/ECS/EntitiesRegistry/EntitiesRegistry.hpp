//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../Entity/Entity.hpp"
#include "../Component/Component.hpp"
#include "../../Core/Log/Logger.hpp"

#include <unordered_map>
#include <stack>

namespace Engine
{
#pragma region PROTO

    template <typename TComponent>
    class ComponentSystem
    {
    public:
        ComponentSystem()
        {
            for(std::size_t i = 0; i < 50; ++i)
            {
                AvailableRecords.push(Record(i));
            }
        }

        TComponent* Add(EntityHandle entityHandle)
        {
            if(AvailableRecords.empty())
            {
                throw std::runtime_error("No available component in pool");
            }

            auto componentRecord = AvailableRecords.top();
            AvailableRecords.pop();

            Instances[componentRecord.index] = TComponent();
            EntitiesRecords[entityHandle] = componentRecord;

            return &Instances[componentRecord.index];
        }

        void Remove(EntityHandle entityHandle)
        {
            auto record = EntitiesRecords[entityHandle];
            Instances[record.index].OnDestroy();
            Instances[record.index].~TComponent();

            AvailableRecords.push(record);
        }

        bool Has(EntityHandle entityHandle)
        {
            return EntitiesRecords.template contains(entityHandle);
        }

        TComponent* Get(EntityHandle entityHandle)
        {
            if(!Has(entityHandle))
            {
                return nullptr;
            }

            auto record = EntitiesRecords[entityHandle];
            if(record.index < 0 || record.index >= Instances.count())
            {
                throw std::runtime_error("invalid record index");
            }

            return &Instances[record.index];
        }

    private:
        struct Record
        {
            inline explicit Record(std::size_t index)
                    : index(index) {}
            std::size_t index;
        };

        // TODO : Use a dynamic Array instead ?
        std::array<TComponent, 50> Instances;
        std::stack<Record> AvailableRecords;

        std::unordered_map<EntityHandle, Record> EntitiesRecords;
    };

    // TODO : Don't use that
    static constexpr std::size_t MaxComponentsPerPools = 50;

#pragma endregion PROTO

    /* Entities registry is the core storage of our ESC system
     * it manages Components allocation & association with an Entity
     * */
    class EntitiesRegistry
    {
    private:

        // TODO : use non static solution for Type -> value mapping
        template <typename TComponent>
        ComponentSystem<TComponent>& GetSystem()
        {
            static ComponentSystem<TComponent> system;
            return system;
        }

    public:

        EntityHandle CreateEntity();

        // TODO : Implement Entity Destruction and related Components pointers reset (explorer pointer ?)
        void DestroyEntity(EntityHandle handle);

        template<typename T>
        T* AddComponentTo(EntityHandle entityHandle);

        template<typename T>
        T* GetComponentOf(EntityHandle entityHandle);

        template<typename T>
        bool HasComponent(EntityHandle entityHandle);

        template<typename T>
        void RemoveComponentOf(EntityHandle entityHandle);
    };

} // Engine

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
