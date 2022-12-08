#include <Engine/Engine.hpp>
#include <PathfindingSystem/PathfindingSystem.hpp>

#include "PathfindingSystem/BFS/TNode.h"

void testNodes();

int main()
{

}



struct Tile2D
{
    Tile2D() = default;
    Tile2D(int id, int x, int y, bool isWall = false) : _id(id), _x(x), _y(y), _isWall(isWall) {}

    int _id{ 0 };
    int _x{ 0 }, _y{ 0 };
    bool _isWall{ false };

    bool operator==(const Tile2D& tile) const
    {
        return _id == tile._id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tile2D& tile) // Why "friend" here and not on == operator ?
    {
        os << tile._x << "_" << tile._y;
        return os;
    }
};

class LoggerNodes
{
public:
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        WeakSharedPtr<T> pWeakNode(pSharedNode);

        std::cout << pWeakNode.lock()->GetContent() << " ---> ";
    }
};

void testTNode()
{
    // =========== Map with int
    constexpr int MAP_WIDTH = 12;
    constexpr int MAP_HEIGHT = 7;

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

    //constexpr int MAP_WIDTH = 5;
    //constexpr int MAP_HEIGHT = 5;

    //constexpr int defaultMap[MAP_HEIGHT][MAP_WIDTH] =
    //{
    //    { 0, 0, 0, 0, 1 },
    //    { 0, 1, 1, 0, 0 },
    //    { 0, 0, 0, 1, 1 },
    //    { 0, 1, 0, 0, 0 },
    //    { 0, 0, 0, 1, 0 },
    //};

    LoggerNodes loggerNodes;

    // =========== Construct node map
    using NodeTile2D = TNode<Tile2D>;
    NodeTile2D tileMap2D[MAP_HEIGHT][MAP_WIDTH];
    TGraph<Tile2D> g;

    int tilesID = 0;

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            tileMap2D[l][c].SetContent(Tile2D(tilesID, c + 1, l + 1, static_cast<bool>(defaultMap[l][c])));

            g.AddNodes(tileMap2D[l][c]);

            tilesID++;
        }
    }

    for (int l = 0; l < MAP_HEIGHT; ++l)
    {
        for (int c = 0; c < MAP_WIDTH; ++c)
        {
            if (tileMap2D[l][c].GetContent()._isWall)
                continue; // Walls have no neighbors

            if (c - 1 > 0 && !(tileMap2D[l][c - 1].GetContent()._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c - 1]);

            if (l - 1 > 0 && !(tileMap2D[l - 1][c].GetContent()._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l - 1][c]);

            if (c + 1 < MAP_WIDTH && !(tileMap2D[l][c + 1].GetContent()._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l][c + 1]);

            if (l + 1 < MAP_HEIGHT && !(tileMap2D[l + 1][c].GetContent()._isWall))
                g.AddEdge(tileMap2D[l][c], tileMap2D[l + 1][c]);
        }
    }

    g.AddEdge(tileMap2D[0][2], tileMap2D[7][10]);

    // ----- Init

    using QueueNodeInt = std::queue<std::shared_ptr<NodeTile2D>>;

    QueueNodeInt queueNodesVisited = QueueNodeInt();
    queueNodesVisited.push(g.FindNode(tileMap2D[0][0]));

    queueNodesVisited.back()->SetIsVisitedBy(queueNodesVisited.back());

    // ---- BFS, display result

    g.TraversalGraphRecursifBreathFirst(g.FindNode(tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]), queueNodesVisited, loggerNodes);

    g.VisitParentsFrom(g.FindNode(tileMap2D[MAP_HEIGHT - 1][MAP_WIDTH - 1]), loggerNodes);
}