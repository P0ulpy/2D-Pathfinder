set(GAME_HEADERS
        includes/main.hpp
        )

set(GAME_SOURCES
        src/main.cpp
        )

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/PathfindingSystem)
target_link_libraries(Game Engine PathfindingSystem)

install(TARGETS Game RUNTIME DESTINATION bin)
