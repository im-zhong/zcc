// 2023/3/6
// zhangzhong

#include "cgen/register_allocation.h"
#include "cgen/cgen.h"
#include "cgen/liveness_analysis.h"
#include <memory>

namespace cgen {

InstructionList RegisterAllocator::allocate() {
    // 1. 生成cfg
    // 这个工作应该由ControlFlowGraph的构造函数完成
    auto cfg = make_cfg(_list);

    while (true) {
        // 2. 然后就开始那个循环
        // 2. liveness analysis 那只有cfg可以进行这个分析
        // 分析的结果是什么, 我们就可以得到一个冲突图
        auto interfere_graph = cfg->analyse();

        // 3. 然后对冲突图进行着色
        if (interfere_graph->coloring()) {
            // 如果成功着色那么就直接返回cfg的指令即可
            return cfg->emit_asm();
        }

        cfg->spill();
    }

    // 然后我们可能无法成功着色 此时需要选取一个寄存器进行spill
}

std::unique_ptr<RegisterAllocator>
make_register_allocator(InstructionList list) {
    return std::make_unique<RegisterAllocator>(list);
}

} // namespace cgen