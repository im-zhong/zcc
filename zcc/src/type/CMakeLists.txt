project(type)

add_library(type STATIC
    qualifier_type.cpp
    type_check.cpp
    type_to_string.cpp
    type.cpp)
target_include_directories(type PUBLIC include)
target_link_libraries(type
    PUBLIC zcc_util
    PUBLIC core
    PUBLIC symbol)

add_executable(test_type test_type.cpp)
target_link_libraries(test_type
    PUBLIC zcc_util
    PUBLIC type
)


