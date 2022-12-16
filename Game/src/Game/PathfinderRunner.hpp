//
// Created by Flo on 16/12/2022.
//

#ifndef PATHFINDER_PATHFINDERRUNNER_HPP
#define PATHFINDER_PATHFINDERRUNNER_HPP

#include <queue>
#include <chrono>
#include "SFML/System/Vector2.hpp"
#include "TGraph.h"
#include "Utilities/Algo.h"
#include "Utilities/NodeFunctors.h"

class PathfinderRunner
{
public:
    ~PathfinderRunner()
    {
        delete graphMap;
    }

    struct PathfinderResults
    {
        std::chrono::time_point<std::chrono::steady_clock> startTimer;
        std::chrono::time_point<std::chrono::steady_clock> endTimer;
        std::list<std::shared_ptr<TNode<Tile2D>>> visitedList;
        std::list<std::shared_ptr<TNode<Tile2D>>> pathList;
        bool valid = false;
    };

    void Reset(const sf::Vector2u& mapSize)
    {
        delete graphMap;
        portal1 = { -1, -1 };
        portal2 = { -1, -1 };
        portals.clear();

        this->mapSize = mapSize;

        graphMap = new GraphMap2D(mapSize.x, mapSize.y);

        graphMap->RemoveWallAt(Tile2D(0, 0));
        graphMap->RemoveWallAt(Tile2D(mapSize.x - 1, mapSize.y - 1));

        beginNode = graphMap->GetGraph().FindNode(Tile2D(0,0));
        endNode = graphMap->GetGraph().FindNode(Tile2D(mapSize.x - 1, mapSize.y - 1));

        // Values Reset
        queueNodesVisited = std::queue<NodeSharedPtr<Tile2D>>();

        // AStar
        functorAStarManhattanVisited = FunctorVisitedList<Tile2D>();
        functorAStarEuclideanVisited = FunctorVisitedList<Tile2D>();

        // BFS
        functorBFSFinalPathNodes = FunctorVisitedList<Tile2D>();
        functorBFSVisited = FunctorVisitedList<Tile2D>();
    }

    void SetRandomWall(int chanceForNoWall = 7)
    {
        graphMap->SetRandomWallsForMap(chanceForNoWall);
    }

    void InitBFS()
    {
        queueNodesVisited.push(beginNode);
        queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());
    }

    PathfinderResults RunBFS()
    {
        // Too large map can cause a stack overflow while using BFS Pathfinder
        if(mapSize.x * mapSize.y > 2500)
            return {};

        const auto startTimer = std::chrono::high_resolution_clock::now();
        BFS<Tile2D>::RunBFS(endNode, queueNodesVisited, functorBFSVisited);
        const auto endTimer = std::chrono::high_resolution_clock::now();

        graphMap->GetGraph().VisitParentsFrom(endNode, functorBFSFinalPathNodes);
        std::ranges::reverse(functorBFSFinalPathNodes._listVisited);

        graphMap->GetGraph().ResetParentsForAllNodes();

        return {
            startTimer,
            endTimer,
            functorBFSVisited._listVisited,
            functorBFSFinalPathNodes._listVisited,
            true
        };
    }

    PathfinderResults RunAStarManhattan()
    {
        const auto startTimer = std::chrono::high_resolution_clock::now();
        const auto aStarManhattanResult = AStar<Tile2D>::RunAStar<decltype(Heuristic::manhattan)>(beginNode, endNode, Heuristic::manhattan, functorAStarManhattanVisited);
        const auto endTimer = std::chrono::high_resolution_clock::now();

        return {
            startTimer,
            endTimer,
            functorAStarManhattanVisited._listVisited,
            aStarManhattanResult,
            true
        };
    }

    PathfinderResults RunAStarEuclidean()
    {
        const auto startTimer = std::chrono::high_resolution_clock::now();
        const auto aStarEuclideanResult = AStar<Tile2D>::RunAStar<decltype(Heuristic::euclidean)>(beginNode, endNode, Heuristic::euclidean, functorAStarEuclideanVisited);
        const auto endTimer = std::chrono::high_resolution_clock::now();

        return {
            startTimer,
            endTimer,
            functorAStarEuclideanVisited._listVisited,
            aStarEuclideanResult,
            true
        };
    }

    inline GraphMap2D* GetGraphMap() { return graphMap; };

public:
    sf::Vector2i portal1 { -1, -1 };
    sf::Vector2i portal2 { -1, -1 };
    std::vector<sf::Vector2i> portals;

private:
    sf::Vector2u mapSize;

    // Graph

    GraphMap2D* graphMap = nullptr;
    std::shared_ptr<TNode<Tile2D>> beginNode = nullptr;
    std::shared_ptr<TNode<Tile2D>> endNode = nullptr;

    // BFS
    std::queue<NodeSharedPtr<Tile2D>> queueNodesVisited {};
    FunctorVisitedList<Tile2D> functorBFSFinalPathNodes;
    FunctorVisitedList<Tile2D> functorBFSVisited;

    // AStar Manhattan
    FunctorVisitedList<Tile2D> functorAStarManhattanVisited;

    // AStar Euclidean
    FunctorVisitedList<Tile2D> functorAStarEuclideanVisited;
};

#endif //PATHFINDER_PATHFINDERRUNNER_HPP
