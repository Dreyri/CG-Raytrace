cmake_minimum_required(VERSION 3.6)

project(Raytrace CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(assimp REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(src)
add_subdirectory(test)