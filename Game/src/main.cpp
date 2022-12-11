#include <Engine/Engine.hpp>
#include <PathfindingSystem/PathfindingSystem.hpp>

#include "src/BFS/TGraph.h"
#include "src/BFS/TNode.h"
#include "src/BFS/BFSUtils.h"

int main()
{
    static constexpr int MAP_WIDTH = 12;
    static constexpr int MAP_HEIGHT = 7;

    constexpr int defaultMap[MAP_HEIGHT][MAP_WIDTH] =
    {
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }, // Start at 0,0
        { 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
        { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
        { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 } // Finishes at bottom-right
    };

    // =========== Construct default node map
    using NodeTile2D = TNode<Tile2D>;

    Tile2D tileMap2D[MAP_HEIGHT][MAP_WIDTH];
    TGraph<Tile2D> g;

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            tileMap2D[l][c] = Tile2D(c, l, static_cast<bool>(defaultMap[l][c]));

            g.AddNodes(tileMap2D[l][c]);
        }
    }

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

    g.RemoveAllEdges(tileMap2D[6][10]); // Adding a wall

    // ----- Init

    auto queueNodesVisited = std::queue<std::shared_ptr<NodeTile2D>>();

    queueNodesVisited.push(g.FindNode(tileMap2D[0][0]));

    queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());

    // ---- BFS, display result
    LoggerNodes loggerNodes;

    g.TraversalGraphRecursifBreathFirst(g.FindNode(tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]), queueNodesVisited, loggerNodes);

    g.VisitParentsFrom(g.FindNode(tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]), loggerNodes);
}