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
    static constexpr int MAP_WIDTH = 80;
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
            const auto isWall = !static_cast<bool>(std::rand() % 7);
            tileMap2D[l][c] = Tile2D(c, l, isWall);

            std::cout << (isWall ? "X " : " "); // Just to see the map

            g.AddNodes(tileMap2D[l][c]);
        }
        std::cout << std::endl;
    }

    tileMap2D[0][0]._isWall = false;
    tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]._isWall = false;

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            if (tileMap2D[l][c]._isWall)
                continue; // Walls have no neighbors

            if (c - 1 > 0 && !(tileMap2D[l][c - 1]._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c - 1]);

            if (l - 1 > 0 && !(tileMap2D[l - 1][c]._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l - 1][c]);

            if (c + 1 < MAP_WIDTH && !(tileMap2D[l][c + 1]._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c + 1]);
            
            if (l + 1 < MAP_HEIGHT && !(tileMap2D[l + 1][c]._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l + 1][c]);
        }
    }

    // Just adding a portal
    g.AddEdge(tileMap2D[0][2], tileMap2D[6][8]);

    // Remove the portal
    g.RemoveEdge(tileMap2D[0][2], tileMap2D[6][8]);

    // g.RemoveAllEdges(tileMap2D[6][10]); // Adding a wall

    // ----- Init
    const auto beginNode = g.FindNode(tileMap2D[0][0]);
    const auto endNode = g.FindNode(tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]);

    // ---- BFS
    auto queueNodesVisited = std::queue<std::shared_ptr<NodeTile2D>>();
    queueNodesVisited.push(beginNode);
    queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());

    LoggerNodes loggerNodes;

    std::cout << std::endl;
    std::cout << "BFS: " << std::endl;

    const auto startTimer1 = std::chrono::high_resolution_clock::now();
    g.TraversalGraphRecursifBreathFirst(endNode, queueNodesVisited, loggerNodes);
    const auto endTimer1 = std::chrono::high_resolution_clock::now();

    g.VisitParentsFrom(endNode, loggerNodes);

    std::cout << std::endl;
    std::cout << "AStar: " << std::endl;

    // --- Run AStar
    constexpr AStar aStar;

    const auto startTimer2 = std::chrono::high_resolution_clock::now();
    const auto aStarResult = aStar.RunAStar(beginNode, endNode);
    const auto endTimer2 = std::chrono::high_resolution_clock::now();

    //g.VisitParentsFrom(endNode, loggerNodes);

    for (const auto node : aStarResult)
    {
        std::cout << node->GetContent() << " ---> ";
    }

    const auto durationTimer1 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer1 - startTimer1);
    const auto durationTimer2 = std::chrono::duration_cast<std::chrono::microseconds>(endTimer2 - startTimer2);

    std::cout << std::endl;
    std::cout << "Duration timer BFS: " << durationTimer1 << std::endl;
    std::cout << "Duration timer AStar: " << durationTimer2 << std::endl;
}