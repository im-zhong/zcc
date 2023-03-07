// 2023/3/5
// zhangzhong
// 测试用例

// 先从简单的开始 我就构造一条instruction
// 然后你给我生成汇编指令 但是不带寄存器名字
#include "cgen/cgen.h"
#include "cgen/liveness_analysis.h"
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

void test_branch() {
    // using ir::IR;

    // // 总共有三种分支
    // auto branch = ir::make_branch(
    //     ir::Branch{.condition =
    //                    ir::Symbol{
    //                        .name = "%t1",
    //                        .type = ir::make_basic_type(IR::BOOL),
    //                    },
    //                .true_label = "%true"});

    // auto gen = cgen::make_x86_cgen();
    // auto codes = gen->emit_asm(branch);
    // EXPECT_EQ(codes.size(), 2);
    // EXPECT_EQ(codes[0].emit(), "movq %t1, %t3");
    // EXPECT_EQ(codes[1].emit(), "addq %t2, %t3");
}

void test_cond() {}

void test_liveness_analysis() {
    cgen::ControlFlowGraph graph{cgen::InstructionList{}};
    graph.init();
    graph.print();
    graph.liveness_analysis();
    graph.print_inout();
}

void test_liveness_analysis2() {
    cgen::ControlFlowGraph graph{cgen::InstructionList{}};

    graph.analyse();
}

int main(int argc, char* argv[]) {
    test_simple();
    test_branch();
    test_cond();
    test_liveness_analysis();
    test_liveness_analysis2();
}
