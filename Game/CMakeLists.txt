set(GAME_HEADERS
        #includes/main.hpp
        #GraphMap2D.h
)

set(GAME_SOURCES
        #src/main.cpp
        src/Game/main.cpp
)

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES} ${ENGINE_HEADERS} ${ENGINE_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/PathfindingSystem)

target_link_libraries(Game Engine PathfindingSystem)

install(TARGETS Game RUNTIME DESTINATION bin)
