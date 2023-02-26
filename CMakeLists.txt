cmake_minimum_required(VERSION 3.25.0)
project(zccproject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_VERBOSE_MAKEFILE on)


add_subdirectory(util)
add_subdirectory(zcc)
add_subdirectory(tool)

# 其实也不需要install
# install(TARGETS zcc DESTINATION "."
#         RUNTIME DESTINATION bin
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib
#         )
