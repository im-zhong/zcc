project(ast)

add_library(ast STATIC
    expression.cpp)
target_include_directories(ast PUBLIC include)
target_link_libraries(ast 
    PUBLIC core
    PUBLIC scan)

add_executable(test_tree test_tree.cpp)
target_link_libraries(test_tree
    PRIVATE core
    PRIVATE ast)

add_executable(test_expression test_expression.cpp)
target_link_libraries(test_expression
    PRIVATE core
    PRIVATE ast)
