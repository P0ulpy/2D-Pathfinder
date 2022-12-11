#pragma once

#include "TGraph.h"

struct Tile2D
{
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

struct LoggerNodes
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        NodeWeakPtr<T> pWeakNode(pSharedNode);

        std::cout << pWeakNode.lock()->GetContent() << " ---> ";
    }
};