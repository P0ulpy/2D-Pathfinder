//
// Created by Flo on 16/12/2022.
//

#ifndef PATHFINDER_TILESRENDERER_HPP
#define PATHFINDER_TILESRENDERER_HPP

#include <cstdint>
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Color.hpp"
#include "TGraph.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class TilesRenderer
{
public:
    TilesRenderer(sf::RenderWindow* window, const sf::Vector2u& mapSize, const uint32_t panelWidth)
        : window(window)
        , mapSize(mapSize)
        , panelWidth(panelWidth)
    {
        tilesSize = GetTilesSize();
        tilesBorderWidth = 5;

        tilesShape.setFillColor(sf::Color(101, 72,  50));
        tilesShape.setSize({
               (float)tilesSize - (float)tilesBorderWidth,
               (float)tilesSize - (float)tilesBorderWidth
       });
    }

    void RenderTiles(GraphMap2D& graphMap)
    {
        for(uint32_t x = 0; x < mapSize.x; ++x)
        {
            for(uint32_t y = 0; y < mapSize.y; y++)
            {
                SetTileAt(x, y);

                if(graphMap.IsTileAWall(Tile2D(x, y)))
                {
                    tilesShape.setFillColor(sf::Color(113, 113,  113));
                }
                else if(false)
                {
                    // Portal
                    tilesShape.setFillColor(sf::Color(129, 172,  255));
                }
                else
                {
                    tilesShape.setFillColor(sf::Color(73, 117, 55));
                }

                window->draw(tilesShape);
            }
        }
    }

    void RenderVisited(const std::list<std::shared_ptr<TNode<Tile2D>>>& list)
    {
        auto defaultTilesSize = tilesShape.getSize();
        sf::Vector2f newTilesSize = { defaultTilesSize.x / 2, defaultTilesSize.y / 2 };
        sf::Vector2f offset = { newTilesSize.x / 2, newTilesSize.y / 2 };
        tilesShape.setSize(newTilesSize);

        for(auto& visitedTile : list)
        {
            auto pos = visitedTile->GetContent()._pos;
            SetTileAt(pos.x, pos.y, offset);

            tilesShape.setFillColor(sf::Color::Magenta);

            window->draw(tilesShape);
        }

        tilesShape.setSize(defaultTilesSize);
    }

    void RenderPath(const std::list<std::shared_ptr<TNode<Tile2D>>>& list)
    {
        auto defaultTilesSize = tilesShape.getSize();
        sf::Vector2f newTilesSize = { defaultTilesSize.x / 2, defaultTilesSize.y / 2 };
        sf::Vector2f offset = { newTilesSize.x / 2, newTilesSize.y / 2 };
        tilesShape.setSize(newTilesSize);

        for(auto& visitedTile : list)
        {
            auto pos = visitedTile->GetContent()._pos;
            SetTileAt(pos.x, pos.y, offset);

            tilesShape.setFillColor(sf::Color::Red);

            window->draw(tilesShape);
        }

        tilesShape.setSize(defaultTilesSize);
    }

    void RenderPortals(const std::vector<sf::Vector2i>& portals)
    {
        for(auto& portalPos : portals)
        {
            auto pos = portalPos;
            SetTileAt(pos.x, pos.y);

            tilesShape.setFillColor(sf::Color(129, 172, 255));

            window->draw(tilesShape);
        }
    }

    void SetTileAt(uint32_t x, uint32_t y, sf::Vector2f offset = { 0.f, 0.f })
    {
        sf::Vector2f position {
                (float)x * (float)tilesSize,
                (float)y * (float)tilesSize
        };

        position += { (float)tilesBorderWidth / 2, (float)tilesBorderWidth / 2 };
        position.x += (float)panelWidth;

        position += offset;

        tilesShape.setPosition(position);
    }

    uint32_t GetTilesSize() const
    {
        auto windowSize = window->getSize();
        windowSize.x -= panelWidth;

        return (windowSize.x > windowSize.y) ? windowSize.x / mapSize.x : windowSize.y / mapSize.y;
    }

private:
    sf::RenderWindow* window = nullptr;

    sf::RectangleShape tilesShape;

    sf::Vector2u mapSize { 0, 0 };

    uint32_t panelWidth = 0;

    uint32_t tilesSize = 0;
    uint32_t tilesBorderWidth = 5;
};

#endif //PATHFINDER_TILESRENDERER_HPP
