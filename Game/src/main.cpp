#include <iostream>
#include <Engine/Engine.hpp>

#include "Utilities/Algo.h"
#include "Utilities/NodeFunctors.h"
#include "TGraph.h"

constexpr int RAND_MAP_W = 50;
constexpr int RAND_MAP_H = 25;

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

int main()
{
    using NodeTile2D = TNode<Tile2D>;

    Tile2D tileMap2D[FINAL_MAP_H][FINAL_MAP_W]; // Used to construct a default graph easier
    TGraph<Tile2D> g;

    // =========== Construct default node map
    std::srand(std::time(nullptr));

    for (int l = 0; l < FINAL_MAP_H; ++l)
    {
        for (int c = 0; c < FINAL_MAP_W; ++c)
        {
            const auto isTraversable = static_cast<bool>((USE_DEF_MAP ? !DEF_MAP[l][c] : std::rand() % 6));
            const auto newTile = Tile2D(c, l, isTraversable);

            tileMap2D[l][c] = newTile;
            g.AddNodes(newTile);

            std::cout << (!isTraversable ? "X " : "  "); // Just to see the map
        }

        std::cout << std::endl;
    }

    // =========== Start and finish points are necessary traversable
    tileMap2D[0][0]._isTraversable = true;
    tileMap2D[FINAL_MAP_H - 1][FINAL_MAP_W - 1]._isTraversable = true;

    // =========== Linking every node to their neighbors, if they are traversable
    for (int l = 0; l < FINAL_MAP_H; ++l)
    {
        for (int c = 0; c < FINAL_MAP_W; ++c)
        {
            if (!tileMap2D[l][c]._isTraversable)
                continue; // Not traversable -> no neighbors

            if (c - 1 > 0 && tileMap2D[l][c - 1]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c - 1]);

            if (l - 1 > 0 && tileMap2D[l - 1][c]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l - 1][c]);

            if (c + 1 < FINAL_MAP_W && tileMap2D[l][c + 1]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c + 1]);
            
            if (l + 1 < FINAL_MAP_H && tileMap2D[l + 1][c]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l + 1][c]);
        }
    }

    // Test adding a portal
    g.AddEdge(Tile2D(3, 4), Tile2D(11, 1));

    // Test remove the portal
    g.RemoveEdge(Tile2D(3, 4), Tile2D(11, 1));

    // Test adding a wall
    g.RemoveAllEdges(Tile2D(9, 0));

    // =========== Init Pathfinder
    const auto beginNode = g.FindNode(Tile2D(0,0));
    const auto endNode = g.FindNode(Tile2D(FINAL_MAP_W - 1, FINAL_MAP_H - 1));

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

    std::cout << "\nCustomLoggerNode: " << std::endl;
    g.VisitParentsFrom(endNode, customLoggerNode);

    std::cout << "\n\nDefautlLoggerNode: " << std::endl;
    g.VisitParentsFrom(endNode, defaultLoggerNode);

    g.VisitParentsFrom(endNode, functorBFSFinalPathNodes);

    /* reset Parents */
    g.ResetParentsForAllNodes();

    std::cout << "\n\nAStar: " << std::endl;

    const auto startTimer2 = std::chrono::high_resolution_clock::now();
    const auto aStarResult = AStar::RunAStar(beginNode, endNode);
    const auto endTimer2 = std::chrono::high_resolution_clock::now();

    g.VisitParentsFrom(endNode, defaultLoggerNode);

    // =========== Display timers result
    const auto durationTimer1 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer1 - startTimer1).count();
    const auto durationTimer2 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer2 - startTimer2).count();

    std::cout << std::endl << std::endl;
    std::cout << "Duration timer BFS: " << durationTimer1 << " us. Node count in the path: " << static_cast<int>(functorBFSFinalPathNodes._listVisited.size()) << std::endl;
    std::cout << "Duration timer AStar: " << durationTimer2 << " us. Node count in the path: " << static_cast<int>(aStarResult.size()) << std::endl;

    return 0;
}
