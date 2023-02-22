cmake_minimum_required(VERSION 3.15)
project(zcc CXX)



add_executable(zcc src/zcc.cpp src/main.cpp)



install(TARGETS zcc DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
