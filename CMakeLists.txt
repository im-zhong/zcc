cmake_minimum_required(VERSION 3.25.0)
project(zccproject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_VERBOSE_MAKEFILE on)

# add doctest recipe没有更新到 2.0 非常遗憾 没法用
# find_package(doctest)
# link to it like target_link_library(xxx doctest::doctest)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)
find_package(doctest REQUIRED)

add_subdirectory(util)
add_subdirectory(zcc)
add_subdirectory(tool)

# 其实也不需要install
# install(TARGETS zcc DESTINATION "."
#         RUNTIME DESTINATION bin
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib
#         )
