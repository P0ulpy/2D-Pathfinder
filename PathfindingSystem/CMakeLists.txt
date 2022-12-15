set(PATHFINDING_SYSTEM_HEADERS
        Utilities/Algo.h
        Utilities/NodeFunctors.h
        Utilities/NodeElements.h
        TGraph.h
        TNode.h)

set(PATHFINDING_SYSTEM_SOURCES
        TNode.cpp
        )

add_library(PathfindingSystem STATIC ${PATHFINDING_SYSTEM_HEADERS} ${PATHFINDING_SYSTEM_SOURCES})