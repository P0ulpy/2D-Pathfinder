set(PATHFINDING_SYSTEM_HEADERS
        BFS/BFSUtils.h
        TGraph.h
        TNode.h
        AStar/AStar.h)

set(PATHFINDING_SYSTEM_SOURCES
        AStar/AStar.cpp
        BFS/BFS.cpp
        TGraph.cpp
        TNode.cpp
        )

add_library(PathfindingSystem STATIC ${PATHFINDING_SYSTEM_HEADERS} ${PATHFINDING_SYSTEM_SOURCES})