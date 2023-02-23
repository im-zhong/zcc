project(ast)

add_library(ast STATIC
    expression.cpp
    statement.cpp
    statement_structure.cpp
    declaration_sematic.cpp
    parser.cpp)
target_include_directories(ast PUBLIC include)
target_link_libraries(ast 
    PUBLIC core
    PUBLIC scan
    PUBLIC type
    PUBLIC symbol)

add_executable(test_tree test_tree.cpp)
target_link_libraries(test_tree
    PRIVATE core
    PRIVATE ast)

add_executable(test_expression test_expression.cpp)
target_link_libraries(test_expression
    PRIVATE core
    PRIVATE ast)

add_executable(test_statement test_statement.cpp)
target_link_libraries(test_statement
    PRIVATE core
    PRIVATE ast
    )
