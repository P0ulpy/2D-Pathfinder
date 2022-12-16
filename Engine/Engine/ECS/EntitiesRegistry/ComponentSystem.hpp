//
// Created by Flo on 15/12/2022.
//

#ifndef PATHFINDER_COMPONENTSYSTEM_HPP
#define PATHFINDER_COMPONENTSYSTEM_HPP

#include "IComponentSystem.hpp"
#include "../../Core/Logger/Logger.hpp"

#include <sstream>

namespace Engine
{
    static constexpr std::size_t MaxComponentsPerPools = 50;

    template <class TComponent>
    class ComponentSystem : public IComponentSystem
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
            inline Entry(Record record)
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

        TComponent* Add(EntityHandle entityHandle, Scene* scene) override
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
            Instances[componentRecord.index].value.component.m_entity = Entity(entityHandle, scene);

            if constexpr (std::is_member_function_pointer_v<decltype(&TComponent::OnAwake)>)
                Instances[componentRecord.index].value.component.OnAwake();

            // Add to linked list
            AddToLinkedList(componentRecord, Instances[componentRecord.index]);

            // Add to entities map
            EntitiesRecords[entityHandle].value = componentRecord;

            return &Instances[componentRecord.index].value.component;
        }

        void Remove(EntityHandle entityHandle) override
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

        bool Has(EntityHandle entityHandle) override
        {
            return EntitiesRecords.template contains(entityHandle);
        }

        TComponent* Get(EntityHandle entityHandle) override
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

        void View(ViewCallback callback) override
        {
            Entry currentEntry = headEntry;
            while(currentEntry != nullptr)
            {
                callback(&currentEntry.value);
                currentEntry = currentEntry.nextEntry;
            }
        }

        void Start() override
        {
            if constexpr (!std::is_member_function_pointer_v<decltype(&TComponent::OnStart)>)
                return;

            Entry currentEntry = headEntry;
            while(currentEntry != nullptr)
            {
                currentEntry.value.OnStart();
                currentEntry = currentEntry.nextEntry;
            }
        }

        void Update(const float& deltaTime) override
        {
            if constexpr (!std::is_member_function_pointer_v<decltype(&TComponent::OnUpdate)>)
                return;

            Entry currentEntry = headEntry;
            while(currentEntry != nullptr)
            {
                currentEntry.value.OnUpdate(deltaTime);
                currentEntry = currentEntry.nextEntry;
            }
        }

        void Render(sf::RenderTarget& renderTarget) override
        {
            if constexpr (!std::is_member_function_pointer_v<decltype(&TComponent::OnRender)>)
                return;

            Entry currentEntry = headEntry;
            while(currentEntry != nullptr)
            {
                currentEntry.value.OnRender(renderTarget);
                currentEntry = currentEntry.nextEntry;
            }
        }

    private:
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

    private:
        // TODO : Use a Custom container who is a dynamically allocated array instead to avoid using MaxComponentsPerPools constexpr and implement inside it the linked list and implement iterator
        std::array<Entry, MaxComponentsPerPools> Instances;
        Entry* headEntry = nullptr;

        Record LastAvailableRecord;

        // TODO : Find a solution to map Handles to Records using the same heap allocation than Instances array
        std::unordered_map<EntityHandle, Record> EntitiesRecords;

        friend class EntitiesRegistry;
    };

} // Engine

#endif //PATHFINDER_COMPONENTSYSTEM_HPP
