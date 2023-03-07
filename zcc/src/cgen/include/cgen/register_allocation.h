// 2023/3/6
// zhangzhong

#pragma once

#include "cgen.h"
#include "cgen/liveness_analysis.h"
#include <memory>

namespace cgen {

class RegisterAllocator {
  public:
    RegisterAllocator(InstructionList list) : _list(list), cfg(list) {}
    InstructionList allocate();

  private:
    // 这是传进来的参数
    InstructionList _list;
    ControlFlowGraph cfg;
};

// 可能也不需要这个指针
std::unique_ptr<RegisterAllocator>
make_register_allocator(InstructionList list);

} // namespace cgen
