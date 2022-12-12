#include <Engine/Engine.hpp>
#include <PathfindingSystem/PathfindingSystem.hpp>

#include "src/BFS/TGraph.h"
#include "src/BFS/TNode.h"
#include "src/BFS/BFSUtils.h"
#include "src/AStar.h"


//static constexpr int MAP_WIDTH = 12;
//static constexpr int MAP_HEIGHT = 7;
//
//constexpr int defaultMap[MAP_HEIGHT][MAP_WIDTH] =
//{
//    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }, // Start at 0,0
//    { 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
//    { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0 },
//    { 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
//    { 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
//    { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
//    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 } // Finishes at bottom-right
//};

int main()
{
    static constexpr int MAP_WIDTH = 30;
    static constexpr int MAP_HEIGHT = 30;

    // =========== Construct default node map
    using NodeTile2D = TNode<Tile2D>;

    Tile2D tileMap2D[MAP_HEIGHT][MAP_WIDTH];
    TGraph<Tile2D> g;

    std::srand(std::time(nullptr));

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            const auto isTraversable = static_cast<bool>(std::rand() % 7);
            tileMap2D[l][c] = Tile2D(c, l, isTraversable);

            std::cout << (!isTraversable ? "X " : " "); // Just to see the map

            g.AddNodes(tileMap2D[l][c]);
        }
        std::cout << std::endl;
    }

    tileMap2D[0][0]._isTraversable = true;
    tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]._isTraversable = true;

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            if (!tileMap2D[l][c]._isTraversable)
                continue; // Not traversable -> no neighbors

            if (c - 1 > 0 && tileMap2D[l][c - 1]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c - 1]);

            if (l - 1 > 0 && tileMap2D[l - 1][c]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l - 1][c]);

            if (c + 1 < MAP_WIDTH && tileMap2D[l][c + 1]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c + 1]);
            
            if (l + 1 < MAP_HEIGHT && tileMap2D[l + 1][c]._isTraversable)
                g.AddEdge(tileMap2D[l][c], tileMap2D[l + 1][c]);
        }
    }

    // Just adding a portal
    g.AddEdge(Tile2D(0, 2), Tile2D(6, 8));

    // Remove the portal
    g.RemoveEdge(Tile2D(0, 2), Tile2D(6, 8));

    // Adding a wall
    g.RemoveAllEdges(Tile2D(6, 10));

    // ----- Init
    const auto beginNode = g.FindNode(Tile2D(0,0));
    const auto endNode = g.FindNode(Tile2D(MAP_HEIGHT - 1, MAP_WIDTH - 1));

    // ---- BFS
    std::cout << std::endl;
    std::cout << "BFS: " << std::endl;

	auto queueNodesVisited = std::queue<std::shared_ptr<NodeTile2D>>();
    queueNodesVisited.push(beginNode);
    queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());

    LoggerNodes loggerNodes;

    const auto startTimer1 = std::chrono::high_resolution_clock::now();
    g.TraversalGraphRecursifBreathFirst(endNode, queueNodesVisited, loggerNodes);
    const auto endTimer1 = std::chrono::high_resolution_clock::now();

    g.VisitParentsFrom(endNode, loggerNodes);

    // --- Run AStar
    std::cout << std::endl;
    std::cout << "AStar: " << std::endl;

    const auto startTimer2 = std::chrono::high_resolution_clock::now();
    const auto aStarResult = AStar::RunAStar(beginNode, endNode);
    const auto endTimer2 = std::chrono::high_resolution_clock::now();

    g.VisitParentsFrom(endNode, loggerNodes);

    //for (const auto& node : aStarResult) // Same as g.VisitParentsFrom
    //{
    //    std::cout << node->GetContent() << " ---> ";
    //}

    // Display timers result
    const auto durationTimer1 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer1 - startTimer1).count();
    const auto durationTimer2 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer2 - startTimer2).count();

    std::cout << std::endl;
    std::cout << "Duration timer BFS: " << durationTimer1 << " ms" << std::endl;
    std::cout << "Duration timer AStar: " << durationTimer2 << " ms" << std::endl;
}