project(scan)

add_library(scan STATIC
    token.cpp)
target_include_directories(scan PUBLIC include)
target_link_libraries(scan PUBLIC core)

add_executable(test_scan test_scan.cpp)
target_link_libraries(test_scan
    PRIVATE core
    PRIVATE scan)
