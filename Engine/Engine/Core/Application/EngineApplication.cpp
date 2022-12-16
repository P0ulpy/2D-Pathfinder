#include "EngineApplication.hpp"
#include "../Time.hpp"

Engine::EngineApplication::EngineApplication()
    : m_window(sf::RenderWindow(
    sf::VideoMode(500, 500),
    "Engine Window",
    sf::Style::Close | sf::Style::Resize))
    , m_sceneLayer(&m_window)
{
    PushLayer(&m_sceneLayer);
}

Engine::EngineApplication::~EngineApplication()
{
    RemoveLayer(&m_sceneLayer);
}

void Engine::EngineApplication::PushLayer(Engine::ApplicationLayer *layer)
{
    m_layers.push_back(layer);
    layer->OnAttach();
}

void Engine::EngineApplication::RemoveLayer(Engine::ApplicationLayer *layer)
{
    m_layers.erase(std::find(m_layers.begin(), m_layers.end(),layer));
    layer->OnDetach();
}

void Engine::EngineApplication::Run()
{
    m_window.display();

    while (m_window.isOpen() && m_running)
    {
        Timestep time = Time::GetTime();
        Timestep timeStep = time - m_LastFrameTime;

        // TODO : Process inputs
        // Input::Process(m_window);

        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnUpdate(timeStep);
        }

        // TODO : Support ImGui
#ifdef IMGUI_SUPPORT
        m_imGuiLayer.Begin();
        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
#endif

        // TODO : Update Window events
        //m_window->OnUpdate();
    }
}

void Engine::EngineApplication::Stop()
{
    m_running = false;
}
