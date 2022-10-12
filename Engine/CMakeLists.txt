set(ENGINE_HEADERS
        Engine/Engine.hpp
        )

set(ENGINE_SOURCES
        src/Engine.cpp
        )

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
add_library(Engine ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(Engine PUBLIC sfml-system sfml-network sfml-graphics sfml-window)

install(TARGETS Engine
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )