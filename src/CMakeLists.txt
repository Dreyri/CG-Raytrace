add_executable(example
               example.cpp)
target_include_directories(example PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${ASSIMP_LIBRARIES} glm Qt5::Gui)