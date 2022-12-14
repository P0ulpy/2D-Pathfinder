#pragma once

#include <queue>
#include <list>
#include <iostream>

#include "NodeElements.h"
#include "../TGraph.h"

struct Heuristic
{
    static Vec2i getDelta(Vec2i source_, Vec2i target_)
    {
        return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
    }

    static double manhattan(Vec2i source, Vec2i target)
    {
        const auto& delta = getDelta(source, target);
        return static_cast<double>(10 * (delta.x + delta.y));
    }

    static double euclidean(Vec2i source_, Vec2i target_)
    {
        const auto& delta = getDelta(source_, target_);
        return static_cast<double>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
    }
};

template<typename NodeType>
class AStar
{
public:
    using SharedPtrNode = std::shared_ptr<TNode<NodeType>>;

    AStar() = default;

    template<typename HeuristicMethod, typename Callable>
    static std::list<SharedPtrNode> RunAStar(const SharedPtrNode& startingNode, const SharedPtrNode& goalNode, HeuristicMethod& heuristic_compute, Callable& functor)
    {
        std::map<SharedPtrNode, SharedPtrNode> Parents;

        SharedPtrNode currentNode = nullptr;

        /* list of node that need to be visited */
        std::list<SharedPtrNode> openedList;

        /* list of node that are already visited */
        std::list<SharedPtrNode> closedList;

        std::list<SharedPtrNode> returnPath = {};

        if (startingNode == nullptr || goalNode == nullptr)
            return returnPath;

        openedList.push_back(startingNode);

        while (!openedList.empty())
        {
            currentNode = *openedList.begin();

            /* Find node in openedList with lowest f */
            for (const auto& currNodeInList : openedList)
            {
	            if (currNodeInList->GetContent().GetF() <= currentNode->GetContent().GetF()) {
                    currentNode = currNodeInList;

                    functor.Visit(currentNode);
                }
            }

            /* if found goalNode, we break */
            if (currentNode == goalNode) 
            {
                while (currentNode != nullptr)
                {
                    returnPath.push_back(currentNode);
                    if (Parents.find(currentNode) == Parents.end())
                        break;
                    currentNode = Parents[currentNode];
                }

                openedList.clear();
                closedList.clear();

                std::reverse(returnPath.begin(), returnPath.end());
                break;
            }

            if (std::ranges::find(closedList, currentNode) == closedList.end())
                closedList.push_back(currentNode);

            openedList.remove(currentNode);

            for (auto& neighbor : currentNode->GetNeighbors())
            {
                if (std::ranges::find(closedList, neighbor) == closedList.end())
                {
                    auto newContent = NodeType(neighbor->GetContent());
                    newContent.SetG(currentNode->GetContent().g + 1.0);
                    newContent.SetH(heuristic_compute(neighbor->GetContent()._pos, goalNode->GetContent()._pos));
                    neighbor->SetContent(newContent);

                    Parents.insert({ neighbor, currentNode });

                    if (std::ranges::find(openedList, neighbor) == openedList.end())
                        openedList.push_back(neighbor);
                }
            }
        }
        return returnPath;
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


namespace Utilities
{
    using NodeSharedPtrTile2D = NodeSharedPtr<Tile2D>;

    void displayPathTile2D(std::list<NodeSharedPtrTile2D> path,const int width, const int height, GraphMap2D& g)
    {
        for (int i = 0; i < width + 2; ++i)
        {
            std::cout << "X ";
        }
        std::cout << std::endl;

        bool isFound = false;
        for (int y = 0; y < height; ++y)
        {
            std::cout << "X ";

            for (int x = 0; x < width; ++x)
            {
                isFound = false;
                if (!g.GetGraph().FindNode(Tile2D(x, y))->GetContent()._isTraversable)
                {
                    std::cout << "X ";
                    continue;
                }
                
                for (auto& it : path)
                {
                    if (it->GetContent()._pos == Vec2i(x, y))
                    {
                        std::cout << "o ";
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                    std::cout << "  "; // Just to see the map
            }

            std::cout << "X ";
            std::cout << std::endl;
        }

        for (int i = 0; i < width + 2; ++i)
        {
            std::cout << "X ";
        }
        std::cout << std::endl;
    }
}