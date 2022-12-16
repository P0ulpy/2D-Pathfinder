//
// Created by Flo on 16/12/2022.
//

#include <SFML/Graphics.hpp>
#include "TilesRenderer.hpp"
#include "PathfinderRunner.hpp"

uint32_t GetPanelWidth(const sf::Vector2u& windowSize)
{
    return windowSize.x / 6;
}

sf::RenderWindow window(sf::VideoMode(1800, 900), "Pathfinder-2D", sf::Style::Close);

const sf::Vector2u mapSize { 50, 30 };

TilesRenderer tilesRenderer(
        &window,
        mapSize,
        GetPanelWidth(window.getSize())
);

PathfinderRunner pathfinderRunner;

void ProcessEvents()
{
    sf::Event event {};
    while(window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:

                sf::Vector2f mousePos = { (float)event.mouseButton.x, (float)event.mouseButton.y };
                mousePos.x -= (float)GetPanelWidth(window.getSize());

                auto tilesSize = tilesRenderer.GetTilesSize();
                sf::Vector2f posInGraph = { mousePos.x / tilesSize, mousePos.y / tilesSize };

                if(posInGraph.x < 0 || posInGraph.x >= mapSize.x ||
                    posInGraph.y < 0 || posInGraph.y >= mapSize.y)
                break;

                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        pathfinderRunner.GetGraphMap()->AddWallAt(Tile2D((int)posInGraph.x, (int)posInGraph.y));
                        break;
                    case sf::Mouse::Right:

                        if(pathfinderRunner.portal1.x == -1 || pathfinderRunner.portal1.y == -1)
                        {
                            pathfinderRunner.portal1 = { (int)posInGraph.x, (int)posInGraph.y };
                            pathfinderRunner.portals.push_back(pathfinderRunner.portal1);
                        }
                        else if(pathfinderRunner.portal2.x == -1 || pathfinderRunner.portal2.y == -1)
                        {
                            pathfinderRunner.portal2 = { (int)posInGraph.x, (int)posInGraph.y };
                            pathfinderRunner.portals.push_back(pathfinderRunner.portal2);

                            pathfinderRunner.GetGraphMap()->AddPortal(Tile2D(pathfinderRunner.portal1.x, pathfinderRunner.portal1.y), Tile2D(pathfinderRunner.portal1.x, pathfinderRunner.portal1.y));

                            pathfinderRunner.portal1 = { -1, -1 };
                            pathfinderRunner.portal2 = { -1, -1 };
                        }

                        break;
                    case sf::Mouse::Middle:
                        break;
                }

                break;
        }
    }
}

int main()
{
    pathfinderRunner.Reset(mapSize);

    PathfinderRunner::PathfinderResults pathfinderResults;

    while (window.isOpen())
    {
        ProcessEvents();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            pathfinderResults = pathfinderRunner.RunAStarEuclidean();
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            pathfinderResults = pathfinderRunner.RunAStarManhattan();
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            pathfinderRunner.InitBFS();
            pathfinderResults = pathfinderRunner.RunBFS();
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            pathfinderRunner.Reset(mapSize);
            pathfinderResults = {};
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            pathfinderRunner.Reset(mapSize);
            pathfinderRunner.SetRandomWall();
            pathfinderResults = {};
        }

        // Render

        window.clear(sf::Color(101, 72,  50));

        tilesRenderer.RenderTiles(*pathfinderRunner.GetGraphMap());
        tilesRenderer.RenderPortals(pathfinderRunner.portals);

        tilesRenderer.RenderVisited(pathfinderResults.visitedList);
        tilesRenderer.RenderPath(pathfinderResults.pathList);

        window.display();
    }

    return 0;
}