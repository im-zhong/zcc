project(ast)

add_library(ast INTERFACE)
target_include_directories(ast INTERFACE include)

add_executable(test_tree test_tree.cpp)
target_link_libraries(test_tree
    PRIVATE core
    PRIVATE ast)
