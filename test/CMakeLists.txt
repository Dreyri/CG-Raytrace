find_package(Catch2 REQUIRED)

function(cg_add_test target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} Catch2::Catch)
  add_test(NAME ${target} COMMAND ${target})
endfunction(cg_add_test target)

cg_add_test(test_example test_example.cpp)