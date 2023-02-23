project(util)

add_library(${PROJECT_NAME} STATIC
    util.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)
