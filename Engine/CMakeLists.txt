set(ENGINE_HEADERS
        Engine/Core/Application/EngineApplication.hpp
        Engine/ECS/Entity/Entity.hpp
        Engine/ECS/Component/Component.hpp
        Engine/Core/Scene/Scene.hpp
        Engine/Core/UUID.hpp
        Engine/Engine.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.hpp
        Engine/Core/Application/ApplicationLayer.hpp
        Engine/Core/Time.hpp
        Engine/Core/Application/LayerStack.hpp
        Engine/Core/Logger/Logger.hpp Engine/Core/Application/SceneLayer.cpp Engine/Core/Application/SceneLayer.hpp Engine/Core/Logger/ConsoleColor.hpp)

set(ENGINE_SOURCES
        Engine/Core/Application/EngineApplication.cpp
        Engine/ECS/Entity/Entity.cpp
        Engine/ECS/Component/Component.cpp
        Engine/Core/Scene/Scene.cpp
        Engine/Core/UUID.cpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.cpp
        Engine/Core/Application/ApplicationLayer.cpp
        Engine/Core/Logger/Logger.cpp Engine/Core/Application/SceneLayer.cpp Engine/Core/Application/SceneLayer.hpp Engine/Core/Logger/ConsoleColor.hpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_library(Engine
        STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(Engine
        PUBLIC sfml-system sfml-network sfml-graphics sfml-window)