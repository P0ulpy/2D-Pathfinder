//
// Created by Flo on 16/12/2022.
//

#include <SFML/Graphics.hpp>
#include <chrono>
#include <functional>
#include "TGraph.h"
#include "Utilities/NodeElements.h"
#include "Utilities/Algo.h"
#include "Utilities/NodeFunctors.h"

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

GraphMap2D graphMap(mapSize.x, mapSize.y);

void SetTileAt(float x, float y)
{
    sf::Vector2f position {
            (float)x * (float)tilesSize,
            (float)y * (float)tilesSize
    };

    position += { (float)tilesBorderWidth / 2, (float)tilesBorderWidth / 2 };

    tilesShape.setPosition(position);
}

void RenderTiles()
{
    for(uint32_t x = 0; x < mapSize.x; ++x)
    {
        for(uint32_t y = 0; y < mapSize.y; y++)
        {
            SetTileAt((float)x, (float)y);

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

            window.draw(tilesShape);
        }
    }
}

void RenderVisited(const std::list<std::shared_ptr<TNode<Tile2D>>>& list)
{
    for(auto& visitedTile : list)
    {
        auto pos = visitedTile->GetContent()._pos;
        SetTileAt(pos.x, pos.y);

        tilesShape.setFillColor(sf::Color::Magenta);

        window.draw(tilesShape);
    }
}

void RenderPath(const std::list<std::shared_ptr<TNode<Tile2D>>>& list)
{
    for(auto& visitedTile : list)
    {
        auto pos = visitedTile->GetContent()._pos;
        SetTileAt(pos.x, pos.y);

        tilesShape.setFillColor(sf::Color::Red);

        window.draw(tilesShape);
    }
}

int main()
{
    InitShape();

    graphMap.SetRandomWallsForMap(7);

    graphMap.RemoveWallAt(Tile2D(0, 0));
    graphMap.RemoveWallAt(Tile2D(mapSize.x - 1, mapSize.y - 1));

    // =========== Init Pathfinder
    const auto beginNode = graphMap.GetGraph().FindNode(Tile2D(0,0));
    const auto endNode = graphMap.GetGraph().FindNode(Tile2D(mapSize.x - 1, mapSize.y - 1));

    // =========== Init functors to visit nodes if needed
    using NodeSharedPtrTile2D = NodeSharedPtr<Tile2D>;

    const std::function<void(NodeSharedPtrTile2D)> functionStringLoggerNode = [](const NodeSharedPtrTile2D& node)
    {
        std::cout << node->GetContent() << " -> ";
    };

    FunctorVisitedList<Tile2D> functorBFSFinalPathNodes;
    FunctorVisitedList<Tile2D> functorBFSVisited;

    FunctorVisitedList<Tile2D> functorAStarManhattanVisited;
    FunctorVisitedList<Tile2D> functorAStarEuclideanVisited;

    FunctorCustomLoggerNodes customLoggerNode(functionStringLoggerNode);

    // =========== Init BFS
    auto queueNodesVisited = std::queue<NodeSharedPtrTile2D>();
    queueNodesVisited.push(beginNode);
    queueNodesVisited.back()->SetIsVisitedByParent(queueNodesVisited.back());

    // =========== Run BFS
    const auto startTimerBFS = std::chrono::high_resolution_clock::now();
    //BFS<Tile2D>::RunBFS(endNode, queueNodesVisited, functorBFSVisited);
    const auto endTimerBFS = std::chrono::high_resolution_clock::now();

    graphMap.GetGraph().VisitParentsFrom(endNode, functorBFSFinalPathNodes);
    std::ranges::reverse(functorBFSFinalPathNodes._listVisited);

    graphMap.GetGraph().ResetParentsForAllNodes();

    // =========== Run AStar manhattan
    const auto startTimerAStarManhattan = std::chrono::high_resolution_clock::now();
    const auto aStarManhattanResult = AStar<Tile2D>::RunAStar<decltype(Heuristic::manhattan)>(beginNode, endNode, Heuristic::manhattan, functorAStarManhattanVisited);
    const auto endTimerAStarManhattan = std::chrono::high_resolution_clock::now();

    // =========== Run AStar euclidean
    graphMap.GetGraph().ResetParentsForAllNodes();

    const auto startTimerAStarEuclidean = std::chrono::high_resolution_clock::now();
    const auto aStarEuclideanResult = AStar<Tile2D>::RunAStar<decltype(Heuristic::euclidean)>(beginNode, endNode, Heuristic::euclidean, functorAStarEuclideanVisited);
    const auto endTimerAStarEuclidean = std::chrono::high_resolution_clock::now();

    while (window.isOpen())
    {
        ProcessEvents();

        window.clear(sf::Color(101, 72,  50));

        RenderTiles();

        RenderVisited(functorAStarEuclideanVisited._listVisited);
        //RenderVisited(functorAStarManhattanVisited._listVisited);

        RenderPath(aStarEuclideanResult);
        //RenderPath(aStarManhattanResult);

        window.display();
    }

    return 0;
}