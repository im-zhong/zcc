project(ir)

add_library(ir STATIC
    src/ir.cpp
    src/driver.cpp
    src/parser.cpp
    src/scanner.cpp
    src/type.cpp)
target_include_directories(ir PUBLIC include)
target_link_libraries(ir PUBLIC fmt::fmt)

add_executable(test_ir src/test_ir.cpp)
target_link_libraries(test_ir 
    PRIVATE ir
    PRIVATE gtest::gtest
    )
