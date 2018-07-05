add_executable(example
               example.cpp)
target_include_directories(example PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${ASSIMP_LIBRARIES} glm Qt5::OpenGL Qt5::Gui rtcore)

add_library(rtcore
            window.cpp
            core/image.cpp
            core/ray.cpp
            core/scene.cpp
            core/sphere.cpp
            core/raytracer_simple.cpp
            core/tracer_sphere.cpp
            core/boundingsphere.cpp)
target_link_libraries(rtcore PRIVATE glm)
