#pragma once

struct TileType
{
    enum Type
    {
        Nothing, Wall, Portal
    };

    int id;

    bool operator=(const TileType& tileType) const
    {
        return id;
    }
};


struct Tile2D
{
    enum TileType
    {
        Nothing, Wall, Portal
    };

    Tile2D() = default;
    Tile2D(int x, int y, bool isWall = false) : _pos(x, y), _isWall(isWall) {}
    Tile2D(const sf::Vector2i point, bool isWall = false) : _pos(point.x, point.y), _isWall(isWall) {}

    sf::Vector2i _pos{ 0, 0 };
    bool _isWall{ false }; // Could be replaced by an int _weight

    bool operator==(const Tile2D& tile) const
    {
        return _pos == tile._pos;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tile2D& tile) // Why "friend" here and not on == operator ?
    {
        os << tile._pos.x << "_" << tile._pos.y;
        return os;
    }
};

struct GraphTile2D
{
    static constexpr int MAP_WIDTH = 12;
    static constexpr int MAP_HEIGHT = 7;

    GraphTile2D(const Tile2D(&arr)[MAP_HEIGHT][MAP_WIDTH]);

    const Tile2D(&_tileMap2D)[MAP_HEIGHT][MAP_WIDTH];
    TGraph<Tile2D> _graph;

    void InitGraph(const Tile2D(&arr)[MAP_HEIGHT][MAP_WIDTH])
    {
        for (size_t l = 0; l < MAP_HEIGHT; l++)
        {
            for (size_t c = 0; c < MAP_WIDTH; c++) {
                _graph.AddNodes(arr[l][c]);
            }
        }
    }

    void UpdateAllTilesNeighbors()
    {
        for (int l = 0; l < MAP_HEIGHT; ++l)
        {
            for (int c = 0; c < MAP_WIDTH; ++c)
            {
                UpdateTileNeighbors(l, c);
            }
        }
    }

    void UpdateTileNeighbors(int l, int c)
    {
        if (_tileMap2D[l][c]._isWall)
            return; // Walls have no neighbors

        if (c - 1 > 0 && !(_tileMap2D[l][c - 1]._isWall))
            _graph.AddEdge(_tileMap2D[l][c], _tileMap2D[l][c - 1]);

        if (l - 1 > 0 && !(_tileMap2D[l - 1][c]._isWall))
            _graph.AddEdge(_tileMap2D[l][c], _tileMap2D[l - 1][c]);

        if (c + 1 < MAP_WIDTH && !(_tileMap2D[l][c + 1]._isWall))
            _graph.AddEdge(_tileMap2D[l][c], _tileMap2D[l][c + 1]);

        if (l + 1 < MAP_HEIGHT && !(_tileMap2D[l + 1][c]._isWall))
            _graph.AddEdge(_tileMap2D[l][c], _tileMap2D[l + 1][c]);
    }

    TGraph<Tile2D>& GetGraph() { return _graph; }
};


struct LoggerNodes
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        WeakSharedPtr<T> pWeakNode(pSharedNode);

        std::cout << pWeakNode.lock()->GetContent() << " ---> ";
    }
};