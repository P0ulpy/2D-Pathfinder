#pragma once

#include "TGraph.h"

struct Tile2D
{
    Tile2D() = default;
    Tile2D(int x, int y, bool isTraversable = true) : _pos(x, y), _isTraversable(isTraversable) {}
    Tile2D(const sf::Vector2i point, bool isWall = true) : _pos(point.x, point.y), _isTraversable(isWall) {}

    sf::Vector2i _pos{ 0, 0 };
    bool _isTraversable{ false }; // Could be replaced by an int _weight

    /* distance from the starting node to a given node */
    double g { 0 };
    /* distance from a given node to the final node */
    double h { 0 };

    void SetG(const double value) { g = value; }
    void SetH(const double value) { h = value; }

    bool operator==(const Tile2D& tile) const
    {
        return _pos == tile._pos;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tile2D& tile) // Why "friend" here and not on == operator ?
    {
        os << tile._pos.x << "_" << tile._pos.y;
        return os;
    }

    /* G + H, sum of the two distances */
    double GetF() const { return g + h; };
};

struct FunctorLoggerNodes
{
    template <typename T>
    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        NodeWeakPtr<T> pWeakNode(pSharedNode);

        if(auto node = pWeakNode.lock())
        {
            std::cout << node->GetContent() << " ---> ";
        }
        else
        {
            std::cout << "null ---> ";
        }
    }
};

template <typename T>
struct FunctorVisitedList
{
    FunctorVisitedList(const int& preallocListVisitedSize = 0)
    {
        _listVisited.resize(preallocListVisitedSize);
    }

	std::list<NodeSharedPtr<T>> _listVisited;

    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        _listVisited.push_front(pSharedNode);
    }
};

template<typename T>
struct FunctorCustomLoggerNodes
{
    FunctorCustomLoggerNodes(const std::function<void(NodeSharedPtr<T>)>& callable) : _callable(callable) {}

    std::function<void(NodeSharedPtr<T>)> _callable;

    void Visit(NodeSharedPtr<T> pSharedNode)
    {
        _callable(pSharedNode);
    }
};