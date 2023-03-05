// 2023/3/5
// zhangzhong
// 测试用例

// 先从简单的开始 我就构造一条instruction
// 然后你给我生成汇编指令 但是不带寄存器名字
#include "cgen/cgen.h"
#include "ir/ir.h"
#include "ir/type.h"
#include <gtest/gtest.h>

void test_simple() {
    using ir::IR;
    auto instruction = ir::make_instruction(ir::Instruction{
        .op = IR::ADD,
        .left =
            ir::Symbol{
                .name = "%t1",
                .type = ir::make_basic_type(IR::I64),
            },
        .right =
            ir::Symbol{
                .name = "%t2",
                .type = ir::make_basic_type(IR::I64),
            },
        .result =
            ir::Symbol{
                .name = "%t3",
                .type = ir::make_basic_type(IR::I64),
            },
    });

    auto gen = cgen::make_x86_cgen();

    // 这个代码需要生成一个新的结构
    // 不对 这个结果应该是一个list
    auto asm_instruction = gen->emit_asm(instruction);

    // 那么我希望它能生成什么指令呢
    EXPECT_EQ(asm_instruction.size(), 2);
    EXPECT_EQ(asm_instruction[0].emit(), "movq %t1, %t3");
    EXPECT_EQ(asm_instruction[1].emit(), "addq %t2, %t3");
}

int main(int argc, char* argv[]) { test_simple(); }
