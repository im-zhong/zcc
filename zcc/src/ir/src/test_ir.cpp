// 2023/2/25
// zhangzhong
// test ir

// 一趟解析结束之后应该生成一份 符号表 类型表 指令表 标号表

#include "ir/driver.h"
#include "ir/ir.h"
#include "ir/parser.hpp"

int main(int argc, char* argv[]) {
    auto drv = ir::driver{};
    // 然后读取用户输入的文件
    for (int i = 1; i < argc; i++) {
        drv.parse(argv[i]);
    }

    // 然后输出
    drv.dump();
    return 0;
}
