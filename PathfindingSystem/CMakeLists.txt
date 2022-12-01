set(PATHFINDING_SYSTEM_HEADERS
        PathfindingSystem/PathfindingSystem.hpp
        )

set(PATHFINDING_SYSTEM_SOURCES
        PathfindingSystem/PathfindingSystem.cpp
        )

add_library(PathfindingSystem STATIC ${PATHFINDING_SYSTEM_HEADERS} ${PATHFINDING_SYSTEM_SOURCES})