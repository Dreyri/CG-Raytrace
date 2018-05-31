cmake_minimum_required(VERSION 3.6)

project(Raytrace CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(assimp REQUIRED)
find_package(glm REQUIRED)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)