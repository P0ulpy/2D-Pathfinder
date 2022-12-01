//
// Created by Flo on 01/12/2022.
//

#ifndef PATHFINDER_SCENELAYER_HPP
#define PATHFINDER_SCENELAYER_HPP

#include "SFML/Graphics/RenderTarget.hpp"
#include "ApplicationLayer.hpp"
#include "../Scene/Scene.hpp"

namespace Engine
{
    class SceneLayer : public ApplicationLayer
    {
    public:
        SceneLayer() = default;
        SceneLayer(sf::RenderTarget* renderTarget, const std::string_view& name = "Layer");

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;

    private:
        Scene m_scene;
        sf::RenderTarget* m_renderTarget = nullptr;
    };

} // Engine

#endif //PATHFINDER_SCENELAYER_HPP
