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

# format
`find . -iname *.h -o -iname *.c -o -iname *.cpp | xargs clang-format -i`

# convert
`iconv -f GB2312 -t UTF-8 zcc/keyword_hash/string_hash.h -o zcc/keyword_hash/string_hash.h`

# finish part
1. core
2. perfect hash
3. scan
4. symbol, symbol table
5. type
6. tree, expression, statement, declaration

# emit llvm ir
`clang -S -emit-llvm src.c`

# 结果conan2.0还是不行 大部分conancenter里面的recipe都是1.x 2.0改了api 没法用了...
