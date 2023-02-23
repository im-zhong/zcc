project(symbol)

# # 我们可以用 add_executable(test) 来作为doctest的单元测试
# # 然后用 add_library 来提供模块的库实现 让其他模块调用
# add_executable(test_symbol_table run.cpp)
# target_include_directories(test_symbol_table PRIVATE include)
# target_link_libraries(test_symbol_table PRIVATE core)

add_library(symbol STATIC
    symbol.cpp)
target_include_directories(symbol PUBLIC include)
target_link_libraries(symbol
    PUBLIC core
    PUBLIC util)
