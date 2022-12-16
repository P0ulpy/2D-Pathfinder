set(GAME_HEADERS
        src/Game/TilesRenderer.hpp
        src/Game/PanelRenderer.hpp
)

set(GAME_SOURCES
        #src/main.cpp
        src/Game/main.cpp
        src/Game/PathfinderRunner.hpp)

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/PathfindingSystem)

target_link_libraries(Game Engine PathfindingSystem)

install(TARGETS Game RUNTIME DESTINATION bin)
