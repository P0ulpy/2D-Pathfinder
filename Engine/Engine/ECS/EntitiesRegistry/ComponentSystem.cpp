//
// Created by Flo on 15/12/2022.
//

#include "ComponentSystem.hpp"

namespace Engine
{
    template<class TComponent>
    ComponentSystem<TComponent>::ComponentSystem() {
        LastAvailableRecord.nextAvailableRecord = Record(0);

        for(std::size_t i = 0; i < MaxComponentsPerPools; ++i)
        {
            if(i + 1 >= MaxComponentsPerPools)
                Instances.push(Entry(Record(-1)));
            else
                Instances.push(Entry(Record(i + 1)));
        }
    }

    template<class TComponent>
    TComponent *ComponentSystem<TComponent>::Add(EntityHandle entityHandle) {
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

    template<class TComponent>
    void ComponentSystem<TComponent>::RemoveFromLinkedList(ComponentSystem::Entry &entry) {
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

    template<class TComponent>
    void ComponentSystem<TComponent>::AddToLinkedList(ComponentSystem::Record record, ComponentSystem::Entry &entry) {
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

    template<class TComponent>
    void ComponentSystem<TComponent>::Update(const float &deltaTime) {
        if constexpr (!std::is_member_function_pointer_v<decltype(&TComponent::OnUpdate)>)
            return;

        Entry currentEntry = headEntry;
        while(currentEntry != nullptr)
        {
            currentEntry.value.OnUpdate(deltaTime);
            currentEntry = currentEntry.nextEntry;
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Start() {
        if constexpr (!std::is_member_function_pointer_v<decltype(&TComponent::OnStart)>)
            return;

        Entry currentEntry = headEntry;
        while(currentEntry != nullptr)
        {
            currentEntry.value.OnStart();
            currentEntry = currentEntry.nextEntry;
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::View(IComponentSystem::ViewCallback callback) {
        Entry currentEntry = headEntry;
        while(currentEntry != nullptr)
        {
            callback(&currentEntry.value);
            currentEntry = currentEntry.nextEntry;
        }
    }

    template<class TComponent>
    TComponent *ComponentSystem<TComponent>::Get(EntityHandle entityHandle) {
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

    template<class TComponent>
    bool ComponentSystem<TComponent>::Has(EntityHandle entityHandle) {
        return EntitiesRecords.template contains(entityHandle);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle) {
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

} // Engine