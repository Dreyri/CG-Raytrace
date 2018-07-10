add_executable(example
               example.cpp)
target_include_directories(example PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${ASSIMP_LIBRARIES} glm Qt5::Gui rtcore)

add_library(rtcore
            core/image.cpp
            core/ray.cpp
            core/scene.cpp
            core/sphere.cpp
            core/raytracer_simple.cpp
            core/tracer_sphere.cpp
            core/boundingsphere.cpp)
target_link_libraries(rtcore PRIVATE glm)

add_library(rtpath
  path/renderer.cpp
  path/objects.cpp
  path/scene.cpp
  path/kdtree.cpp
  path/texture.cpp
  path/material.cpp
  path/camera.cpp
  ../third_party/tiny_obj_loader.cpp
)
target_link_libraries(rtpath PRIVATE glm Qt5::Core Qt5::Gui)
target_include_directories(rtpath PUBLIC ${CMAKE_SOURCE_DIR})

if (OPENMP_FOUND)
  message(STATUS "OpenMP found")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(path
  path.cpp)
target_link_libraries(path PRIVATE glm rtpath Qt5::Core Qt5::Gui)

