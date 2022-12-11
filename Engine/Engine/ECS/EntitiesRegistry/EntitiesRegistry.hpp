//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../Entity/Entity.hpp"
#include "../Component/Component.hpp"
#include "../../Core/Logger/Logger.hpp"

#include <unordered_map>
#include <stack>
#include <sstream>
#include <functional>

namespace Engine
{
#pragma region PROTO

    // TODO : Don't use that use a global allocator for all components Systems
    static constexpr std::size_t MaxComponentsPerPools = 50;

    template <typename TComponent = Component>
    class ComponentSystem
    {
        static_assert(!std::is_abstract_v<TComponent>);
        static_assert(std::is_base_of_v<Component, TComponent>);

    private:
        struct Record
        {
            inline explicit Record(int64_t index)
                    : index(index) {}
            int64_t index = -1;
        };

        struct Entry
        {
            union EntryUnion
            {
                TComponent component;
                Record nextAvailableRecord;
            };

            Entry() = default;
            Entry(Record record)
                : value(record)
            {}

            EntryUnion value;
            Entry* nextEntry = nullptr;
            Entry* prevEntry = nullptr;
        };

    public:
        ComponentSystem()
        {
            LastAvailableRecord.nextAvailableRecord = Record(0);

            for(std::size_t i = 0; i < MaxComponentsPerPools; ++i)
            {
                if(i + 1 >= MaxComponentsPerPools)
                    Instances.push(Entry(Record(-1)));
                else
                    Instances.push(Entry(Record(i + 1)));
            }
        }

        TComponent* Add(EntityHandle entityHandle)
        {
            if(EntitiesRecords.contains(entityHandle))
            {
                // TODO : Refactor Logger:: methods to be variadic
                std::stringstream msg("Can't add Component [");
                msg << typeid(TComponent).name() << "] to entity [" << entityHandle << "] : this Entity already have this component attached to it";

                //Logger::Err("Can't add Component [", typeid(TComponent).name(), "] to entity [", entityHandle, "] : this Entity already have this component attached to it");
                Logger::Err(msg.view());
                return nullptr;
            }

            if(LastAvailableRecord.index == -1)
                throw std::runtime_error("No available component in pool");

            // Save LastAvailableRecord for instanciation system
            Record componentRecord = LastAvailableRecord;
            LastAvailableRecord = Instances[componentRecord].value.nextAvailableRecord;

            // Initialize Component
            Instances[componentRecord.index].value.component = TComponent();
            Instances[componentRecord.index].value.component.m_handle = entityHandle;

            if constexpr (std::is_member_function_pointer_v<decltype(&TComponent::OnAwake)>)
                Instances[componentRecord.index].value.component.OnAwake();

            // Add to linked list
            AddToLinkedList(componentRecord, Instances[componentRecord.index]);

            // Add to entities map
            EntitiesRecords[entityHandle].value = componentRecord;

            return &Instances[componentRecord.index].value.component;
        }

        void Remove(EntityHandle entityHandle)
        {
            Record record = EntitiesRecords[entityHandle];

            if constexpr (std::is_member_function_pointer_v<decltype(&TComponent::OnDestroy)>)
                Instances[record.index].value.component.OnDestroy();

            Instances[record.index].value.component.~TComponent();

            // Bind
            RemoveFromLinkedList(Instances[record.index]);
            EntitiesRecords.erase(entityHandle);

            Instances[record.index].value.nextAvailableRecord = LastAvailableRecord;
            LastAvailableRecord = record;
        }

        bool Has(EntityHandle entityHandle)
        {
            return EntitiesRecords.template contains(entityHandle);
        }

        void AddToLinkedList(Record record, Entry& entry)
        {
            // This mean the array is empty
            if(headEntry == nullptr)
            {
                headEntry = &entry;
                return;
            }

            {
                // find previous neighbour
                Entry* prevEntry = nullptr;
                for(int64_t i = record.index; i >= 0; --i)
                {
                    if(Instances[i].nextEntry)
                    {
                        prevEntry = &Instances[i];
                        break;
                    }
                }

                entry.prevEntry = prevEntry;

                if(prevEntry)
                    prevEntry->nextEntry = &entry;
            }

            {
                // find next neighbour
                Entry* nextEntry = nullptr;
                for(int64_t i = record.index + 1; i < MaxComponentsPerPools; ++i)
                {
                    if(Instances[i].prevEntry)
                    {
                        nextEntry = &Instances[i];
                        break;
                    }
                }

                entry.nextEntry = nextEntry;

                if(nextEntry)
                    nextEntry->prevEntry = &entry;
            }
        }

        void RemoveFromLinkedList(Entry& entry)
        {
            if(entry.nextEntry)
            {
                entry.nextEntry->prevEntry = entry.prevEntry;

                if(&entry == headEntry)
                {
                    headEntry = entry.nextEntry;
                }
            }

            if(entry.prevEntry)
                entry.prevEntry->nextEntry = entry.nextEntry;

            entry.nextEntry = nullptr;
            entry.prevEntry = nullptr;
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

            return &Instances[record.index].value;
        }

        // TODO : Use iterator instead
        using ViewCallback = std::function<void(TComponent*)>;

        void View(ViewCallback callback)
        {
            Entry currentEntry = headEntry;
            while(currentEntry != nullptr)
            {
                callback(&currentEntry.value);
                currentEntry = currentEntry.nextEntry;
            }
        }

    private:
        // TODO : Use a Custom container who is a dynamically allocated array instead to avoid using MaxComponentsPerPools constexpr and implement inside it the linked list and implement iterator
        std::array<Entry, MaxComponentsPerPools> Instances;
        Entry* headEntry = nullptr;

        Record LastAvailableRecord;

        // TODO : Find a solution to map Handles to Records without using the same heap allocation than Instances array
        std::unordered_map<EntityHandle, Record> EntitiesRecords;
    };

#pragma endregion PROTO

    /* Entities registry is the core storage of our ESC system
     * it manages Components allocation & association with an Entity
     * */
    class EntitiesRegistry
    {
    private:

        // TODO : use non static solution for Type -> value mapping
        template <typename TComponent = Component>
        ComponentSystem<TComponent>& GetSystem()
        {
            static ComponentSystem<TComponent> system;
            return system;
        }

    public:

        EntityHandle CreateEntity();

        // TODO : Implement Entity Destruction and related Components pointers reset (explorer pointer ?)
        void DestroyEntity(EntityHandle handle);

        template<typename T = Component>
        T* AddComponentTo(EntityHandle entityHandle);

        template<typename T = Component>
        T* GetComponentOf(EntityHandle entityHandle);

        template<typename T = Component>
        bool HasComponent(EntityHandle entityHandle);

        // TODO : found a way to notify Component destruction to all objects how reference it currently if a Component is removed accessing to his memory address don't throw any exception
        template<typename T = Component>
        void RemoveComponentOf(EntityHandle entityHandle);

        template<typename T = Component>
        void View(ComponentSystem<T>::ViewCallback callback);
    };

} // Engine

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
