#include <iostream>
#include <Engine/Engine.hpp>

#include "Utilities/Algo.h"
#include "Utilities/NodeFunctors.h"

#include "TGraph.h"

constexpr int RAND_MAP_W = 15;
constexpr int RAND_MAP_H = 8;

constexpr int DEF_MAP_W = 12;
constexpr int DEF_MAP_H = 7;

constexpr int DEF_MAP[DEF_MAP_H][DEF_MAP_W] =
{
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, // Start at 0,0
    { 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
    { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
    { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 } // Finishes at bottom-right
};

constexpr bool USE_DEF_MAP = false; // Set it to true to use the default map above

constexpr int FINAL_MAP_H = USE_DEF_MAP ? DEF_MAP_H : RAND_MAP_H;
constexpr int FINAL_MAP_W = USE_DEF_MAP ? DEF_MAP_W : RAND_MAP_W;

void InitMap(GraphMap2D& g);

int main()
{
    using NodeTile2D = TNode<Tile2D>;

    GraphMap2D g(FINAL_MAP_W, FINAL_MAP_H);

    InitMap(g);

    g.DisplayGraphInConsole();

    // =========== Init Pathfinder
    const auto beginNode = g.GetGraph().FindNode(Tile2D(0,0));
    const auto endNode = g.GetGraph().FindNode(Tile2D(FINAL_MAP_W - 1, FINAL_MAP_H - 1));

    using NodeSharedPtrTile2D = NodeSharedPtr<Tile2D>;

    const std::function<void(NodeSharedPtrTile2D)> testFunctionLoggerNode = [](const NodeSharedPtrTile2D& node)
    {
        std::cout << node->GetContent()._isTraversable;
    };

    FunctorLoggerNodes defaultLoggerNode;
    FunctorVisitedList<Tile2D> functorBFSAllNodesVisited(FINAL_MAP_H * FINAL_MAP_W);
    FunctorVisitedList<Tile2D> functorBFSFinalPathNodes;
    FunctorCustomLoggerNodes<Tile2D> customLoggerNode(testFunctionLoggerNode);

    // =========== Run BFS
    std::cout << std::endl << std::endl;
    std::cout << "BFS: " << std::endl;

	auto queueNodesVisited = std::queue<NodeSharedPtrTile2D>();
    queueNodesVisited.push(beginNode);
    queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());

    const auto startTimer1 = std::chrono::high_resolution_clock::now();
    BFS<Tile2D>::RunBFS(endNode, queueNodesVisited, functorBFSAllNodesVisited);
    const auto endTimer1 = std::chrono::high_resolution_clock::now();

    //std::cout << "\nCustomLoggerNode: " << std::endl;
    //g.GetGraph().VisitParentsFrom(endNode, customLoggerNode);

    std::cout << "\n\nDefautlLoggerNode: " << std::endl;
    g.GetGraph().VisitParentsFrom(endNode, defaultLoggerNode);

    g.GetGraph().VisitParentsFrom(endNode, functorBFSFinalPathNodes);

    /* reset Parents */
    g.GetGraph().ResetParentsForAllNodes();

    std::cout << "\n\nAStar: " << std::endl;

    const auto startTimer2 = std::chrono::high_resolution_clock::now();
    const auto aStarResult = AStar::RunAStar(beginNode, endNode);
    const auto endTimer2 = std::chrono::high_resolution_clock::now();

    g.GetGraph().VisitParentsFrom(endNode, defaultLoggerNode);

    // =========== Display timers result
    const auto durationTimer1 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer1 - startTimer1).count();
    const auto durationTimer2 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer2 - startTimer2).count();

    std::cout << std::endl << std::endl;
    std::cout << "Duration timer BFS: " << durationTimer1 << " us. Node count in the path: " << static_cast<int>(functorBFSFinalPathNodes._listVisited.size()) << std::endl;
    std::cout << "Duration timer AStar: " << durationTimer2 << " us. Node count in the path: " << static_cast<int>(aStarResult.size()) << std::endl;

    return 0;
}

void InitMap(GraphMap2D& g)
{
    if (USE_DEF_MAP)
    {
        for (int l = 0; l < FINAL_MAP_H; ++l)
        {
            for (int c = 0; c < FINAL_MAP_W; ++c)
            {
                if (static_cast<bool>(DEF_MAP[l][c]))
                {
                    g.AddWallAt(Tile2D(c, l));
                }
            }
        }
    }
    else
    {
        g.SetRandomWallsForMap(7);

        g.RemoveWallAt(Tile2D(0, 0));
        g.RemoveWallAt(Tile2D(FINAL_MAP_W - 1, FINAL_MAP_H - 1));

        g.AddWallAt(Tile2D(3, 2));
        g.AddWallAt(Tile2D(4, 2));
        g.AddWallAt(Tile2D(5, 2));
        g.AddWallAt(Tile2D(3, 3));
        g.AddWallAt(Tile2D(4, 3));
        g.AddWallAt(Tile2D(5, 3));
        g.AddWallAt(Tile2D(3, 4));
        g.AddWallAt(Tile2D(4, 4));
        g.AddWallAt(Tile2D(5, 4));
        g.RemoveWallAt(Tile2D(4, 3)); // Should display a node alone, surrounded by walls

        g.AddPortal(Tile2D(0, 1), Tile2D(7, 7));
        g.RemovePortal(Tile2D(0, 1), Tile2D(7, 7));
        g.RemovePortal(Tile2D(0, 2), Tile2D(7, 7)); // Doesn't work because no portal
    }
}
