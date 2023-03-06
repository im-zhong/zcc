project(cgen)

add_library(cgen STATIC
    src/cgen.cpp
    src/x86.cpp
    src/register_allocation.cpp
    src/liveness_analysis.cpp)
target_link_libraries(cgen
    PUBLIC ir
    PUBLIC zcc_util
    PUBLIC fmt::fmt)
target_include_directories(cgen 
    PUBLIC include)

add_executable(test_cgen src/test_cgen.cpp)
target_link_libraries(test_cgen
    PRIVATE cgen
    PRIVATE ir
    PRIVATE gtest::gtest)
