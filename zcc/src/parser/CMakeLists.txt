project(parser)

# static parser library
add_library(parser STATIC 
    src/parser.cpp
)
target_include_directories(parser 
    PUBLIC include
)

# parser unit tests
add_executable(test_parser 
    test/test_parser.cpp
)
target_link_libraries(test_parser
    PRIVATE parser
    PRIVATE gtest::gtest
)
