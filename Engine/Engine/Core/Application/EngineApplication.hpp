#ifndef PATHFINDER_ENGINEAPPLICATION_HPP
#define PATHFINDER_ENGINEAPPLICATION_HPP

#include <vector>
#include <memory>
#include "ApplicationLayer.hpp"

namespace Engine
{
    class EngineApplication
    {
    public:
        void Run();
        void Stop();

    private:
        // TODO : Attach SFML Window to application
        //std::unique_ptr<sf::RenderWindow> m_window;

        bool m_running = true;
        Timestep m_LastFrameTime = 0;

        // TODO : use a custom stack where we can iterate throw
        std::vector<ApplicationLayer*> m_layers;
    };
}

#endif //PATHFINDER_ENGINEAPPLICATION_HPP