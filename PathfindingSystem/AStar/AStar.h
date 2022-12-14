#pragma once

#include "../TNode.h"

class AStar
{
public:
    using SharedPtrNodeTile2D = std::shared_ptr<TNode<Tile2D>>;

    AStar() = default;

    const std::list<SharedPtrNodeTile2D> RunAStar(const SharedPtrNodeTile2D& startingNode, const SharedPtrNodeTile2D& goalNode);

    static sf::Vector2i getDelta(sf::Vector2i source_, sf::Vector2i target_);

    static double manhattan(sf::Vector2i source, sf::Vector2i target);
};
