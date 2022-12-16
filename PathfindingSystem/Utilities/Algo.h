#pragma once

#include <queue>
#include <list>

#include "NodeElements.h"
#include "../TGraph.h"

struct AStar
{
    using SharedPtrNodeTile2D = NodeSharedPtr<Tile2D>;

    AStar() = default;

    static std::list<SharedPtrNodeTile2D> RunAStar(const SharedPtrNodeTile2D& startingNode, const SharedPtrNodeTile2D& goalNode)
    {
        /* list of node that need to be visited */
        std::list<SharedPtrNodeTile2D> openedList;

        /* list of node that are already visited */
        std::list<SharedPtrNodeTile2D> closedList;

        openedList.push_back(startingNode);

        startingNode->ResetParent();
        goalNode->ResetParent();

        while (!openedList.empty())
        {
            auto currentNode = openedList.front();

            /* Find node in openedList with lowest f */
            for (const auto& currNodeInList : openedList)
            {
                if (currNodeInList->GetContent().GetF() <= currentNode->GetContent().GetF()) {
                    currentNode = currNodeInList;
                }
            }

            if (std::ranges::find(closedList, currentNode) == closedList.end())
                closedList.push_back(currentNode);

            openedList.remove(currentNode);

            for (auto& neighbor : currentNode->GetNeighbors())
            {
                if (std::ranges::find(closedList, neighbor) == closedList.end())
                {
                    neighbor->GetContent().SetG(currentNode->GetContent().g + 10.0);

                    neighbor->GetContent().SetH(manhattan(neighbor->GetContent()._pos, goalNode->GetContent()._pos));

                    neighbor->SetIsVisitedByParent(currentNode);

                    if (std::ranges::find(openedList, neighbor) == openedList.end())
                        openedList.push_back(neighbor);

                    if (neighbor == goalNode)
                    {
                        std::list<SharedPtrNodeTile2D> path;
                        auto current = neighbor;

                        while (current != nullptr) 
                        {
                            path.push_back(current);
                            current = current->GetParent();
                        }
                        
                        return path;
                    }
                }
            }
        }
        return std::list<SharedPtrNodeTile2D> {};
    }

    static Vec2i getDelta(Vec2i source_, Vec2i target_)
    {
        return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
    }

    static double manhattan(Vec2i source, Vec2i target)
    {
        const auto delta = std::move(getDelta(source, target));
        return 10 * (delta.x + delta.y);
    }
};


template<typename T>
struct BFS
{
    template <typename Callable>
    static void RunBFS(NodeSharedPtr<T> goalNode, std::queue<NodeSharedPtr<T>>& queueNodesToVisit, Callable& Functor)
    {
        if (!queueNodesToVisit.empty())
        {
            auto currentNode = queueNodesToVisit.front();
            Functor.Visit(currentNode);

            if (currentNode == goalNode)
                return; // Found !

            queueNodesToVisit.pop();

            for (const auto& neighbor : currentNode->GetNeighbors())
            {
                if (!neighbor->IsVisitedByParent())
                {
                    neighbor->SetIsVisitedByParent(currentNode); // Put currentNode as parent ref of neighbor
                    queueNodesToVisit.push(neighbor);
                }
            }

            RunBFS(goalNode, queueNodesToVisit, Functor);
        }
    }
};