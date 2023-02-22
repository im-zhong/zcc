# zcc
Zhong's C Compiler

# create
`conan new cmake_exe -d name=xxx -d version=xxx`

# install and build dependencies
`conan install .`

# build
`conan build [--profile=debug|release] .`

# custom command
1. conan clean
2. conan fmt 调用clang-format给所有的代码排版 `conan fmt .`
