set(ENGINE_HEADERS
        Engine/Core/Application/EngineApplication.hpp
        Engine/EntityComponentSystem/Entity/Entity.hpp
        Engine/EntityComponentSystem/Component/Component.hpp
        Engine/Core/Scene/Scene.hpp
        Engine/Core/UUID.hpp
        Engine/Engine.hpp
        Engine/EntityComponentSystem/EntitiesRegistry/EntitiesRegistry.hpp
        Engine/Core/Base.hpp Engine/Core/Application/ApplicationLayer.cpp
        Engine/Core/Application/ApplicationLayer.hpp
        Engine/Core/Time.hpp
        Engine/Core/Application/LayerStack.hpp)

set(ENGINE_SOURCES
        Engine/Core/Application/EngineApplication.cpp
        Engine/EntityComponentSystem/Entity/Entity.cpp
        Engine/EntityComponentSystem/Component/Component.cpp
        Engine/Core/Scene/Scene.cpp
        Engine/Core/UUID.cpp
        Engine/EntityComponentSystem/EntitiesRegistry/EntitiesRegistry.cpp
        Engine/Core/Application/ApplicationLayer.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
add_library(Engine STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(Engine PUBLIC sfml-system sfml-network sfml-graphics sfml-window)

install(TARGETS Engine
        LIBRARY DESTINATION engineLib
        ARCHIVE DESTINATION engineLib
        )