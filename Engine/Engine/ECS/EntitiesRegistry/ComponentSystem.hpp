//
// Created by Flo on 15/12/2022.
//

#ifndef PATHFINDER_COMPONENTSYSTEM_HPP
#define PATHFINDER_COMPONENTSYSTEM_HPP

#include "IComponentSystem.hpp"
#include "../../Core/Logger/Logger.hpp"

#include <sstream>

namespace Engine {

    static constexpr std::size_t MaxComponentsPerPools = 50;

    template <class TComponent = Component>
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
        ComponentSystem();
        TComponent* Add(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle) override;
        bool Has(EntityHandle entityHandle) override;
        TComponent* Get(EntityHandle entityHandle) override;
        void View(ViewCallback callback) override;
        void Start() override;
        void Update(const float& deltaTime) override;
        void Render(sf::RenderTarget& renderTarget) override;

    private:
        void AddToLinkedList(Record record, Entry& entry);
        void RemoveFromLinkedList(Entry& entry);

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
