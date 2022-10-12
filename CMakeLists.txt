cmake_minimum_required(VERSION 3.21)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(Pathfinder)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Engine)
add_subdirectory(Game)