// 2023/3/6
// zhangzhong

#include "cgen/liveness_analysis.h"
#include "cgen/x86.h"
#include "util/graph.h"
#include <bits/ranges_algo.h>
#include <bits/ranges_algobase.h>
#include <list>
#include <memory>
#include <queue>
#include <ranges>
#include <stack>
#include <unordered_map>

namespace cgen {

std::unique_ptr<ControlFlowGraph> make_cfg(InstructionList insts) {
    return std::make_unique<ControlFlowGraph>(insts);
}

std::unique_ptr<InterferenceGraph>
make_interference_graph(ControlFlowGraph& cfg) {
    return std::make_unique<InterferenceGraph>(cfg);
}

// 借助cfg提供的所有信息 初始化一个ig
// 然后对其进行着色
void InterferenceGraph::init() {
    // 冲突图的节点的一个live range 也就是一个virtual register
    // edge 表示两个寄存器同时存活

    // 看来只有clang不支持
    // auto l = std::list{1, 2, 3, 4, 5};
    // auto& rl = l;
    // for (auto i : std::ranges::reverse_view{rl}) {
    //     std::cout << i << std::endl;
    // }

    // 那我们只需要遍历一遍basic block
    // 然后在同时存活的节点之间插入一条边即可
    for (auto& [label, block] : _cfg.get_blocks()) {
        // 1. 每个节点的liveout里面的点肯定都是同时存活的
        // for (auto& inst : std::ranges::reverse_view{block.get_insts()}) {
        // }
        // 因为clang不支持上面这样写 我只能这样写了

        // 复制一份
        auto live_now = block.get_live_out();

        for (auto it = block.get_insts().rbegin();
             it != block.get_insts().rend(); it++) {
            auto& inst = *it;
            // 首先 def会kill live now中的符号

            // def不适合作为一个set 更适合作为一个optional
            if (inst.def) {
                live_now.erase(*inst.def);

                // todo: 对于单纯的move操作， %r1 = %r2 这种
                // 不应该添加冲突的边 这样他们可能会被分配到一个寄存器里面
                // 仍然和def符号冲突的live now中的符号 需要插入一条边
                for (auto& use : inst.use) {
                    graph.insert_edge(*inst.def, use);
                }
            }

            // 然后将use的符号加入live now 中
            for (auto& use : inst.use) {
                live_now.insert(use);
            }
        }
    }
}

bool InterferenceGraph::coloring(int size) {
    // 哎呦 不行哎 我们需要复制一份这个图
    auto back_graph = graph;

    // 首先我们需要找到当前degree最大的节点
    // 并将其从图中取出 删除其所有的边
    // 然后递归的调用本算法
    // 知道最终可以正确的进行着色

    // 首先我们需要一个stack
    std::stack<std::string> regs;

    // 就是找到所有的degree < size的节点 并从图中删除该节点

    return false;
}

void ControlFlowGraph::init_cfg() {
    // 两趟
    // 1. find_leader
    // 2. build_graph

    std::vector<size_t> leaders;
    // 首先 第一句一定是leader
    // 一般而言函数的第一句话一定是包含一个label的
    // 所以这个就隐含在我们的设定里了
    //  leaders.push_back(0);

    // 这里需要一个label -> basic_block的映射
    // 然后有一个图 用label来表示basic_block之间的关系
    // 这两个是需要返回的东西 或者说应该保存在ControlFlowGraph里面的东西
    // std::unordered_map<std::string, BasicBlock> blocks;
    // util::DirectedGraph<std::string> cfg;

    const auto& insts = _insts;

    // 根据我们的设计 如果这个句子是有label的 那么他就是leader
    for (size_t i = 0; i < insts.size(); i++) {
        if (insts[i].has_label()) {
            // 这种设计之下我们就不需要leader了
            // 用leader的话实现的逻辑简单一点
            leaders.push_back(i);
            // cfg.insert_node(insts[i].get_label());
        }
    }

    // step2: 将指令按照leader进行划分 并且添加控制流作为边

    // 用一个lambda作为一个从routine就挺好的
    // auto next_leader = [&leaders]() {
    //     // 作为闭包使用
    //     // 调用pop_front如何
    //     auto leader = leaders.front();
    //     leaders.pop_front();
    //     return leader;
    // };

    for (size_t i = 0; i < leaders.size(); i++) {
        // 一次取出一对
        size_t begin = leaders[i];
        // end 可能是下一个 或者是最后一个
        size_t end = (i + 1) < leaders.size() ? leaders[i + 1] : leaders.size();

        // [begin, end) 表示了一个basic block
        // 那么我们在这里就需要构建一个basic block
        std::string curr_label = insts[begin].get_label();
        cfg.insert_node(curr_label);

        // 然后构建一个basic block
        auto block =
            make_basic_block({insts.begin() + begin, insts.begin() + end});
        blocks.insert({curr_label, block});

        // 然后看看最后一句指令是什么 就可以添加边了
        const auto& inst = insts[end - 1];
        // 跳转指令分为无条件跳转和条件跳转
        // 可以实现一些辅助函数 比如 is_jmp(int)
        // is_cjmp(int)
        if (is_jmp(inst.op)) {
            // 那就看一下label是啥
            if (const auto& label = std::get_if<Label>(&inst.src); label) {
                // current_label -> label->name
                cfg.insert_edge(curr_label, label->name);
            } else {
                // should be a bug
                assert(false);
            }
        } else if (is_cjmp(inst.op)) {
            // 如果是cjmp 那么我们必然有两个分支需要跳转
            // true label 出现在 src中
            // false label 出现在下一个basic block 中

            if (const auto& label = std::get_if<Label>(&inst.src); label) {
                // current_label -> label->name
                cfg.insert_edge(curr_label, label->name);
            } else {
                // should be a bug
                assert(false);
            }

            // false label
            // 首先我们的下一个指令必然是存在label的 否则应该是bug
            assert(end < insts.size());
            std::string next_label = insts[end].get_label();
            assert(!next_label.empty());
            cfg.insert_edge(curr_label, next_label);

        } else if (is_ret(inst.op)) {
            // 那么添加一个 curr_label -> __end__ block的
            cfg.insert_edge(curr_label, "__end__");
        }
    }

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

    // return {};
}

BasicBlock make_basic_block(InstructionList insts) {
    // 1. 就是吧指令都复制过去
    return BasicBlock{.insts = {insts.begin(), insts.end()}};
}

void BasicBlock::analyze_liveness() {
    live_out.clear();
    upward_exports.clear();
    kills.clear();
    for (auto& inst : insts) {
        inst.calculate_defuse();
    }

    for (const auto& inst : insts) {
        for (const auto& use : inst.use) {
            if (!kills.contains(use)) {
                upward_exports.insert(use);
            }
        }
        // for (const auto& def : inst.def) {
        //     kills.insert(def);
        // }

        if (inst.def) {
            kills.insert(*inst.def);
        }
    }
}

} // namespace cgen