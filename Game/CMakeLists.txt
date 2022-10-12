set(GAME_HEADERS
        includes/main.hpp
        )

set(GAME_SOURCES
        src/main.cpp
        )

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})
include_directories(${CMAKE_SOURCE_DIR}/Engine)
target_link_libraries(Game Engine)

install(TARGETS Game RUNTIME DESTINATION bin)