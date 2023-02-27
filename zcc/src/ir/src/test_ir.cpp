// 2023/2/25
// zhangzhong
// test ir

// 一趟解析结束之后应该生成一份 符号表 类型表 指令表 标号表

#include "ir/driver.h"
#include "ir/ir.h"
#include "ir/parser.hpp"
#include "ir/type.h"
#include <cassert>
#include <string>

// 可以在这里写测试函数呀
void test_type() {
    using ir::IR;
    using namespace std::string_literals;

    auto pi = ir::make_pointer_type(make_basic_type(IR::I32));
    assert(ir::to_string(*pi) == "*i32"s);
    assert(ir::type_to_string(pi) == "*i32"s);
}

void test() { test_type(); }

int main(int argc, char* argv[]) {
    test();

    auto drv = ir::driver{};
    // 然后读取用户输入的文件
    for (int i = 1; i < argc; i++) {
        drv.parse(argv[i]);
    }

    // 然后输出
    drv.dump();
    return 0;
}
