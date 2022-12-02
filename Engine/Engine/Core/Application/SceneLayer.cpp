//
// Created by Flo on 01/12/2022.
//

#include "SceneLayer.hpp"

namespace Engine
{
    SceneLayer::SceneLayer(sf::RenderTarget *renderTarget, const std::string_view &name)
        : ApplicationLayer::ApplicationLayer(name)
        , m_renderTarget(renderTarget)
    {}

    void SceneLayer::OnUpdate(Timestep ts)
    {
        m_scene.OnUpdate(ts);
        m_scene.RenderScene(*m_renderTarget);
    }

} // Engine