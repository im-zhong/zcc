// 2023/3/6
// zhangzhong

#include "cgen/liveness_analysis.h"
#include "cgen/cgen.h"
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

    while (true) {
        auto nodes = graph.get_nodes();
        if (nodes.empty()) {

            // 到了这个地方 我们肯定可以正常对寄存器进行着色了
            assign();
            return true;
        }

        bool find_node = false;
        // 就是找到所有的degree < size的节点 并从图中删除该节点
        for (const auto& node : nodes) {
            // node: string
            if (graph.get_degree(node) < size) {
                regs.push(node);
                graph.hide_node(node);
                find_node = true;
                break;
            }
        }

        if (!find_node) {
            // 那么此时必须进行一波spill
            // 根据目前的策略 我们就随机spill一个就行
            // what spill will do??
            // spill(*nodes.begin());
            // 到这里其实就说明着色失败了呀
            return false;
        }
    }

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

void ControlFlowGraph::spill(std::string name) {
    // 遍历所有的指令 如果某条指令use了这个name 那么添加一个load
    // 如果某条指令def了这个name 那么添加一个store
    // 并且在符号表中标记一个内存位置

    int id = 0;

    for (auto& [label, block] : blocks) {
        // for (auto& inst : block.insts) {
        //     // 还是老样子 首先必须得是register才会纳入考虑
        //     if (auto reg = std::get_if<Register>(&inst.src); reg) {
        //         if (reg->reg == name) {
        //             // 那么我们需要修改这个名字
        //             reg->reg += (std::to_string(id++));
        //             // 需要在符号表中重新插入此符号
        //             // 这个玩意最好让symbol 自己实现一个函数
        //             // todo: cgen.add_symbol(new_name, new_symbol);

        //             // 然后我需要在这个指令的前面加上一个load 指令
        //             // 这个load指令需要一个内存位置
        //             // 内存位置需要在符号表中找到
        //             // 同样也需要使用label来引用
        //             // auto storage_table = env.get_storage_table();
        //             // 某个寄存器的大小应该跟指令有关

        //             // auto regsiter_table = env.get_register_table();

        //             // 他们的接口用type是不是更统一一点
        //             // 而且指令中也是使用的type
        //             auto new_reg = env.new_register(inst.type);
        //             // 一个新的寄存器 应该也需要在符号表中分配一个新的寄存器
        //             auto mem = env.new_stack_memory(type_to_size(inst.type));

        //             // 每次使用这个符号 都需要一个新的寄存器名字
        //             auto load = Instruction{.op = X86::MOV,
        //                                     .type = inst.type,
        //                                     .src =
        //                                         Memory{
        //                                             .mem = mem,
        //                                         },
        //                                     .dst = Register{.reg = new_reg}};

        //             // 但是我怎么把这个指令插入inst前面呢?？？？？
        //             // 这个功能只能用迭代器实现 不能用for range
        //         }
        //     }
        // }

        for (auto it = block.insts.begin(); it != block.insts.end(); it++) {

            auto& inst = *it;

            // 还是老样子 首先必须得是register才会纳入考虑
            if (auto reg = std::get_if<Register>(&inst.src); reg) {
                if (reg->reg == name) {
                    // 那么我们需要修改这个名字
                    auto new_reg = env.new_register(inst.type);
                    // 一个新的寄存器 应该也需要在符号表中分配一个新的寄存器
                    auto new_mem =
                        env.new_stack_memory(type_to_size(inst.type));

                    // 需要在符号表中重新插入此符号
                    // 这个玩意最好让symbol 自己实现一个函数
                    // todo: cgen.add_symbol(new_name, new_symbol);
                    reg->reg = new_reg;
                    env.add_symbol(new_reg, Symbol{});

                    // 然后我需要在这个指令的前面加上一个load 指令
                    // 这个load指令需要一个内存位置
                    // 内存位置需要在符号表中找到
                    // 同样也需要使用label来引用
                    // auto storage_table = env.get_storage_table();
                    // 某个寄存器的大小应该跟指令有关

                    // auto regsiter_table = env.get_register_table();

                    // 他们的接口用type是不是更统一一点
                    // 而且指令中也是使用的type

                    // 每次使用这个符号 都需要一个新的寄存器名字
                    auto load = Instruction{
                        .op = X86::MOV,
                        .type = inst.type,
                        .src =
                            Memory{
                                .mem = new_mem,
                            },
                        .dst =
                            Register{
                                .reg = new_reg,
                            },
                    };

                    // 但是我怎么把这个指令插入inst前面呢?？？？？
                    // 这个功能只能用迭代器实现 不能用for range
                    block.insts.insert(it, load);
                }
            }

            // 然后是对def进行考察
            // 如果是这样的呢
            // tody: 感觉会出bug add %1, %1
            if (!inst.dst) {
                continue;
            }

            if (auto reg = std::get_if<Register>(&*inst.dst); reg) {
                // 处理逻辑和上面差不多
                // 只不过我们需要换成store

                if (reg->reg == name) {
                    auto new_reg = env.new_register(inst.type);
                    // 一个新的寄存器 应该也需要在符号表中分配一个新的寄存器
                    auto new_mem =
                        env.new_stack_memory(type_to_size(inst.type));
                    reg->reg = new_reg;
                    // todo: 给新的寄存器添加符号
                    env.add_symbol(new_reg, Symbol{});

                    auto store = Instruction{
                        .op = X86::MOV,
                        .type = inst.type,
                        .src =
                            Register{
                                .reg = new_reg,
                            },
                        .dst =
                            Memory{
                                .mem = new_mem,
                            },
                    };

                    // 虽然很丑 但是能用
                    // 插入本条指令的下一条指令
                    auto next_it = it;
                    next_it++;
                    block.insts.insert(next_it, store);
                }
            }
        }
    }
}

void InterferenceGraph::assign() {
    // simplify阶段成功结束之后 我们就进行着色
    // 我们给寄存器上的色保存在符号表中
    auto colors = std::vector<bool>{};
    colors.resize(size, false);

    while (regs.empty()) {
        // why range dont work??
        std::fill(colors.begin(), colors.end(), false);

        auto reg = regs.top();
        regs.pop();

        // 首先将栈顶的节点重新插入图中
        graph.insert_node(reg);
        // 尽可能选择邻居中没有选择过的最小的寄存器 这样可以尽可能少的使用寄存器

        auto adjs = graph.get_adjacency(reg);
        for (auto& adj : adjs) {
            // 如果某个邻居已经占据了我们想要分配的颜色
            if (auto color = env.get_symbol(adj).color; color != -1) {
                assert(color < size);
                colors[color] = true;
            }
        }

        // 然后我们遍历colors 找出最小的没有被分配的颜色
        for (size_t i = 0; i < colors.size(); i++) {
            if (!colors[i]) {
                // 可以用color i来给当前的reg着色
                // todo: 这里应该是一个引用
                auto symbol = env.get_symbol(reg);
                symbol.color = static_cast<int>(i);
                break;
            }
        }
    }
}

} // namespace cgen
