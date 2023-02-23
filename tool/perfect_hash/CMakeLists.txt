project(perfect_hash)

add_executable(${PROJECT_NAME}
    run.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE util)
