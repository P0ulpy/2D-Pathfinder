set(PATHFINDING_SYSTEM_HEADERS
        includes/PathfindingSystem.hpp
        )

set(PATHFINDING_SYSTEM_SOURCES
        src/PathfindingSystem.cpp
        )

add_library(PathfindingSystem ${PATHFINDING_SYSTEM_HEADERS} ${PATHFINDING_SYSTEM_SOURCES})

install(TARGETS PathfindingSystem
        LIBRARY DESTINATION PathfindingSystemLib
        ARCHIVE DESTINATION PathfindingSystemLib
        )