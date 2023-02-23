project(core)

add_library(${PROJECT_NAME} STATIC
    core.cpp
    file_to_string.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)
