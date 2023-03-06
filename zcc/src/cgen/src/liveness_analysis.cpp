// 2023/3/6
// zhangzhong

#include "cgen/liveness_analysis.h"
#include <queue>

namespace cgen {
std::unique_ptr<ControlFlowGraph> make_cfg(InstructionList insts) {
    // 两趟
    // 1. find_leader
    // 2. build_graph

    std::deque<size_t> leaders;
    // 首先 第一句一定是leader
    // 一般而言函数的第一句话一定是包含一个label的
    // 所以这个就隐含在我们的设定里了
    //  leaders.push_back(0);

    // 根据我们的设计 如果这个句子是有label的 那么他就是leader
    for (size_t i = 0; i < insts.size(); i++) {
        if (insts[i].has_label()) {
            leaders.push_back(i);
        }
    }

    // step2: 将指令按照leader进行划分 并且添加控制流作为边

    // 用一个lambda作为一个从routine就挺好的
    auto next_leader = [&leaders]() {
        // 作为闭包使用
        // 调用pop_front如何
        auto leader = leaders.front();
        leaders.pop_front();
        return leader;
    };

    // auto leader = next_leader();
    // for (size_t i = 0; i < insts.size(); i++) {
    //     while (i < leader)
    //         i++;
    //  }

    // for (auto inst : insts) {
    //     auto myinst = BasicBlockInst{
    //         .inst = inst,
    //     };
    //     myinst.calculate_defuse();

    //     // 然后把这条指令加入到
    // }

    // pass2: 遍历列表查找所有的跳转指令的目标是那个basic block
    // 那我只知道label呀

    // todo: 在实现一个和无向图一样的模板，因为每个block都会有一个label
    // 刚好把这个label作为key即可

    return {};
}

void ControlFlowGraph::find_leaders() {
    // 首先第一句肯定是一个leader
    // 卧槽 设计的太对了哥
    // 传进来的刚好是一个vecotr
    // 那么我们就可以指定一个下标了
}

} // namespace cgen