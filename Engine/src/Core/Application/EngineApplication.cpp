#include "../../../Engine/Core/Application/EngineApplication.hpp"
#include "../../../Engine/Core/Time.hpp"

void Engine::EngineApplication::Run()
{
    while (m_running)
    {
        float time = Time::GetTime();
        Timestep timestep = time - m_LastFrameTime;

        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnUpdate(timestep);
        }

        // TODO : Add renderer

        // TODO : Support ImGui
        /* UmGui Layer
        m_imGuiLayer.Begin();
        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
        */

        // TODO : Update Window events
        //m_window->OnUpdate();
    }
}

void Engine::EngineApplication::Stop()
{
    m_running = false;
}
