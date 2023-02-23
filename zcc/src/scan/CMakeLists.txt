project(scan)

add_executable(${PROJECT_NAME}
    token.cpp
    run.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE core)
