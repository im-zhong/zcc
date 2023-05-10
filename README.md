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

# 关于OO还是variant的思考
1. variant更加适用于多个类型之间没有什么关系 但是却必须放到一起 就是类似一个安全的union
    适用于小型的结构之间，比如ipv4 ipv6, 数据库的一行等等 代码较少 没有统一的接口
2. 如果多个类型之间存在继承关系 或者满足同一个接口 那么都应该使用OO
IR部分的type明显满足同一个接口 自然应该使用OO type系统应该采用类似 type expression 的思路实现

# Prototype + TDD
快速原型配合测试驱动开发是非常好的开发方式
首先应该确认一个小的功能子集，并且分析为了实现这个功能每个模块应该做到什么。然后给每个模块的功能设计测试用例和接口，然后再实现。
之后不断添加功能，直到整个项目完成。

# Prototype I
- main.c

  `int main() {
    return 0;
  }`
- main.ir
  
  `fn @main() -> i32 {
    ret %0:i32
  }`

- main.asm

`main:
    mov eax, 0
    ret
`