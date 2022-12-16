//
// Created by Flo on 16/12/2022.
//

#include <SFML/Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(1800, 900), "Pathfinder-2D", sf::Style::Close);

sf::Vector2u mapSize { 50, 25 };
uint32_t tilesSize = 0;
uint32_t tilesBorderWidth = 5;

sf::RectangleShape tilesShape;

unsigned int GetTilesSize(const sf::Vector2u& mapSize, const sf::Vector2u& windowSize)
{
    return (windowSize.x < windowSize.y) ? windowSize.y : windowSize.x / mapSize.x;
}

void InitShape()
{
    tilesSize = GetTilesSize(mapSize, window.getSize());
    tilesBorderWidth = 5;

    tilesShape.setFillColor(sf::Color(101, 72,  50));
    tilesShape.setSize({
        (float)tilesSize - (float)tilesBorderWidth,
        (float)tilesSize - (float)tilesBorderWidth
    });
}

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
            case sf::Event::Resized:
                InitShape();
                break;
            case sf::Event::KeyPressed:

                switch (event.key.code)
                {
                    case sf::Keyboard::A:
                        break;
                    case sf::Keyboard::Z:
                        break;
                    case sf::Keyboard::E:
                        break;
                    case sf::Keyboard::R:
                        break;
                }

                break;
            case sf::Event::MouseButtonPressed:
                break;
        }
    }
}

void RenderTiles()
{
    for(uint32_t x = 0; x < mapSize.x; ++x)
    {
        for(uint32_t y = 0; y < mapSize.y; y++)
        {
            sf::Vector2f position {
                (float)x * (float)tilesSize,
                (float)y * (float)tilesSize
            };

            position += { (float)tilesBorderWidth / 2, (float)tilesBorderWidth / 2 };

            tilesShape.setPosition(position);
            window.draw(tilesShape);
        }
    }
}

int main()
{
    InitShape();

    while (window.isOpen())
    {
        ProcessEvents();

        window.clear(sf::Color(73, 117, 55));

        RenderTiles();

        window.display();
    }

    return 0;
}