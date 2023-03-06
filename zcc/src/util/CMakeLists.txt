project(zcc_util)

add_executable(test_table test_table.cpp)
target_include_directories(test_table PRIVATE include)

add_library(zcc_util INTERFACE)
target_include_directories(zcc_util INTERFACE include)

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph
    PRIVATE zcc_util
    PRIVATE gtest::gtest)
