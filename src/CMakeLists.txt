add_executable(example
               example.cpp)
target_include_directories(example PRIVATE ${ASSIMP_INCLUDE_DIR})
target_link_libraries(example ${ASSIMP_LIBRARIES})