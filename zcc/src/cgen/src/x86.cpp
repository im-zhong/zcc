// 2023/3/5
// zhangzhong

#include "cgen/x86.h"
#include <unordered_map>

namespace cgen {

std::string inst_to_string(int inst) {
    std::unordered_map<int, std::string> mapping = {
        {X86::MOV, "mov"},
        {X86::ADD, "add"},
    };

    auto result = mapping.find(inst);
    if (result == mapping.end()) {
        return "todo";
    }
    return result->second;
}

} // namespace cgen