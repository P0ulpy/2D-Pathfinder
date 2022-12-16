set(GAME_HEADERS
        includes/main.hpp
        )

set(GAME_SOURCES
        src/main.cpp
        )

#
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_subdirectory(../Engine ${CMAKE_SOURCE_DIR}/Engine)

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES} ${ENGINE_HEADERS} ${ENGINE_SOURCES})

#include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/PathfindingSystem)

#target_link_libraries(Game Engine PathfindingSystem)
target_link_libraries(Game
        PUBLIC PathfindingSystem
        PUBLIC sfml-system sfml-network sfml-graphics sfml-window)

#install(TARGETS Game RUNTIME DESTINATION bin)
