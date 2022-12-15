//
// Created by Flo on 14/12/2022.
//

#ifndef PATHFINDER_RENDERER_HPP
#define PATHFINDER_RENDERER_HPP

#include "SFML/Graphics/RenderTarget.hpp"

namespace Engine
{
    class Renderer
    {
        static void BeginRender(sf::RenderTarget& renderTarget);
        static void EndRender();

    private:
        static sf::RenderTarget& m_currentRenderTarget;
    };

} // Engine

#endif //PATHFINDER_RENDERER_HPP
