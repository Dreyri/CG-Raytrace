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
  path/model.cpp
  path/material.cpp
)
target_link_libraries(rtpath PRIVATE glm)

add_executable(path
  path.cpp)
target_link_libraries(path PRIVATE glm rtpath)

