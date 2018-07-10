add_executable(example
               example.cpp)
target_link_libraries(example PRIVATE glm Qt5::Gui rtcore)

add_executable(rt
			   main.cpp
			   mainwindow.cpp
			   mainwindow.hpp
			   renderboy.cpp
			   renderboy.hpp
			   setup.hpp
			   simple.cpp
			   simple.hpp
			   renderArea.cpp
			   renderArea.hpp)
target_link_libraries(rt PRIVATE glm Qt5::Gui Qt5::Widgets rtcore)

add_library(rtcore
			core/camera.cpp
			core/camera.hpp
			core/light.cpp
			core/light.hpp
			core/material.cpp
			core/material.hpp
			core/mesh.cpp
			core/mesh.hpp
			core/object.cpp
			core/object.hpp
            core/ray.cpp
			core/ray.hpp
			core/raytracer.cpp
			core/raytracer.hpp
			core/raytracer_simple.cpp
			core/raytracer_simple.hpp
            core/scene.cpp
			core/scene.hpp
			core/defs.hpp			
			core/sphere.cpp
			core/sphere.hpp
			core/tiny_obj_loader.hpp)
target_link_libraries(rtcore PRIVATE glm)
