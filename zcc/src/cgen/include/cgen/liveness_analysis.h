// 2023/3/5
// zhangzhong

#pragma once

#include "cgen.h"
#include "util/graph.h"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cassert>
#include <iostream>
#include <memory>
#include <ranges>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

namespace cgen {

// 首先我们要表示一个图
// 然后我们需要从Instruction中生成这张图
// 然后对这个图进行分析

class ControlFlowGraph;

class InterferenceGraph {
  public:
    InterferenceGraph(ControlFlowGraph& cfg) : _cfg(cfg) { init(); }

    // 这个可以给出一个选择, 也就是degree最大的节点
    std::string spill_choice() {
        auto nodes = graph.get_nodes();
        std::string reg = "";
        int degree = 0;
        for (const auto& node : graph.get_nodes()) {
            if (graph.get_degree(node) > degree) {
                degree = graph.get_degree(node);
                reg = node;
            }
        }
        return reg;
    }

    // 分成两个阶段
    // 第一个阶段是
    bool coloring(int size);

    void simplify();
    void assign();
    // 可能会有coalease
    // 不是在这里做的 是在cfg上做的
    // void coalease() {}

  private:
    // 这个东西肯定没法用 我们根本不知道N有多大呀
    // 我们也不应该设定一个图的大小 你怎么知道这个图将来会有多大呢
    // 这个东西没法用啊 我也不知道size有多大呀
    // util::UndirectedGraph<std::string, std::size_t N>

    void init();

    // 不是你进行spill
    // void spill(std::string reg);

    ControlFlowGraph& _cfg;
    util::UndirectedGraph<std::string> graph;
    int size;
    std::stack<std::string> regs;
    Env env;
};

std::unique_ptr<InterferenceGraph>
make_interference_graph(ControlFlowGraph& cfg);

// struct BasicBlockInst {
//     Instruction inst;
//     // def 只能def一个吧
//     std::set<std::string> def;
//     std::set<std::string> use;
//     bool is_assign;

//     // 对于这个类型 我们可以计算它的def和use 根据这个指令
//     void calculate_defuse() {
//         is_assign = false;
//         // 只有register才能有use和def信息
//         if (auto src = std::get_if<Register>(&inst.src); src) {
//             use.insert(src->reg);
//         }

//         if (inst.dst) {
//             is_assign = true;
//             if (auto dst = std::get_if<Register>(&*inst.dst); dst) {
//                 use.insert(dst->reg);
//                 def.insert(dst->reg);
//             }
//         }
//     }
// };

// basic block其实就是一个slice view 一对坐标
struct BasicBlock {
    // 这个玩意实际上就是一整个的instruciot list
    std::list<Instruction> insts;
    // 对于每个basic block 我们需要计算live out
    std::set<std::string> live_out;
    // 可能还有一些其他信息

    // variables that are used in block before beging defined
    std::set<std::string> upward_exports;
    // any variable define in block
    std::set<std::string> kills;

    void analyze_liveness();

    void clear() {
        live_out.clear();
        upward_exports.clear();
        kills.clear();
    }

    std::set<std::string> get_live_in() {
        // in = ue + (out - kill)
        auto live_through = std::vector<std::string>{};
        live_through.resize(live_out.size());
        std::ranges::set_difference(live_out, kills, live_through.begin());

        auto live_in = std::vector<std::string>{};
        live_in.resize(live_through.size() + upward_exports.size());
        std::ranges::set_union(upward_exports, live_through, live_in.begin());

        return std::set<std::string>{live_in.begin(), live_in.end()};
    }

    auto& get_insts() { return insts; }
    auto& get_live_out() { return live_out; }
};

BasicBlock make_basic_block(InstructionList insts);

struct Edge {
    int from;
    int to;
};

// 这个类型就是包含live信息的instruction

class ControlFlowGraph {
  public:
    using size_type = std::size_t;
    using symbol_type = std::string;

    ControlFlowGraph(InstructionList insts) : _insts(insts) { init_cfg(); }

    // 当我们着色完成之后 通过这个函数返回重排或者重命名之后的指令
    InstructionList emit_asm() { return {}; }

    // spill一个随机的寄存器 那些复杂的优化规则太麻烦了
    void spill(std::string symbol);

    std::unordered_map<std::string, BasicBlock>& get_blocks() {
        return this->blocks;
    }

    // todo: 我们并没有考虑register class
    // 整数和浮点数是不可能发生冲突的
    std::unique_ptr<InterferenceGraph> analyse() {

        // 对所有的basic block进行一个分析
        for (auto& [_, block] : blocks) {
            block.clear();
            // 先把所有节点的ue 和 kill 计算出来
            block.analyze_liveness();
        }

        // 迭代
        bool is_done = false;
        // std::set<symbol_type> difference{};
        // size_t current_size;
        // std::vector<symbol_type> difference;
        // node_ptr successor_node_ptr = nullptr;
        // node_ptr current_node_ptr = nullptr;
        // for (const auto& node : nodes) {
        //     node->in.clear();
        //     node->out.clear();
        // }
        // 只要没有计算完毕就一直计算
        while (!is_done) {
            is_done = true;
            // 哦没法逆序遍历
            // 逆序遍历
            // 其实这个逆序遍历也是可以做到的
            // 我们可以让图提供一个rpo_iterator
            // 然后我们遍历这个iterator就可以非常快速的收敛了
            // 倒着算 更好
            // 不行啊 blocks是hashmap 没有顺序一说
            // 真想优化还是上面这个方案靠谱一些
            for (auto& [label, block] : blocks) {
                // 拿到该block的所有后继
                // 先算out
                // 找到node的所有后继结点
                for (const auto& succ_label : cfg.get_adjacency(label)) {
                    // 将所有后继结点的successor_node.in -> node.out
                    // int syccessor_node
                    // successor_node_ptr = nodes[successor_node];
                    auto& succ = blocks[succ_label];

                    // current_size = current_node_ptr->out.size();
                    size_t curr_size = block.live_out.size();

                    // 然后就是计算Liveout
                    // out = all succ's in
                    // in = ue + (out - def)
                    auto live_in = succ.get_live_in();
                    // 然后将live in 加入到当前节点的live out中
                    block.live_out.insert(live_in.begin(), live_in.end());

                    // 然后检查是否添加了新的元素
                    if (block.live_out.size() > curr_size) {
                        // 说明添加了新的元素
                        is_done = false;
                    }
                }

                // 下面这一块是重新计算in
            }
        }

        // 然后算完之后 返回一个interface图
        // 这样设计不好 这里应该算一个完整的单元 我们可以添加测试用例
        return make_interference_graph(*this);
    }

    // todo
    // 消除 %r1 = %r1
    void coalease() {}

    // 在寄存器分配的过程中 我们仍然需要一个符号表
    // 因为我们需要给这些符号分配寄存器

    // 稍微包装一下这个节点

    // using node_type = BasicBlock;
    // std::vector<node_type> nodes;
    // // 邻接矩阵
    // std::vector<std::list<size_type>> graph;

    // // 这个可以检查是否有某条边
    // std::vector<Edge> edges;

    // size_type add_node(BasicBlock block) {
    //     nodes.push_back(block);
    //     return nodes.size() - 1;
    // }
    // void add_edge(Edge edge) {
    //     // 首先edge的两个节点我们都要有
    //     assert(edge.from < nodes.size());
    //     assert(edge.to < nodes.size());
    //     graph[edge.from].push_back(edge.to);
    // }

    // 不需要delete
    // BasicBlock get_node(int id);

    // 最起码我现在确定了这个liverange是一句一句的算

    // 但是这个其实并不是node类型
    // 这个只是basic block里面的指令类型
    struct ControlFlowGraphNode {
        std::set<size_type> predecessor;
        std::set<size_type> successor;

        // 这个定义是非常非常合适的 我们应该把问题抽象成只有def和use
        std::set<symbol_type> def;
        std::set<symbol_type> use;

        std::set<symbol_type> in;
        std::set<symbol_type> out;
    };

    // 提供几个graph的接口

    // 如何计算define 和 use
    // 因为中间代码部分未实现 所以使用一个模拟函数进行参数的输入

    void init() {
        node_ptr node1 = std::make_shared<node_type>();
        node_ptr node2 = std::make_shared<node_type>();
        node_ptr node3 = std::make_shared<node_type>();
        node_ptr node4 = std::make_shared<node_type>();
        node_ptr node5 = std::make_shared<node_type>();
        node_ptr node6 = std::make_shared<node_type>();

        node1->successor.insert({1});
        node2->predecessor.insert({0, 4});
        node2->successor.insert({2});
        node3->predecessor.insert({1});
        node3->successor.insert({3});
        node4->predecessor.insert({2});
        node4->successor.insert({4});
        node5->predecessor.insert({3});
        node5->successor.insert({1, 5});
        node6->predecessor.insert({4});

        node1->def.insert({"a"});
        node2->use.insert({"a"});
        node2->def.insert({"b"});
        node3->use.insert({"b", "c"});
        node3->def.insert({"c"});
        node4->use.insert({"b"});
        node4->def.insert({"a"});
        node5->use.insert({"a"});
        node6->use.insert({"c"});

        nodes.push_back(node1);
        nodes.push_back(node2);
        nodes.push_back(node3);
        nodes.push_back(node4);
        nodes.push_back(node5);
        nodes.push_back(node6);
    }

    void print() {
        using namespace std;
        for (size_t i = 0; i < nodes.size(); i++) {
            cout << "node" << i + 1 << endl;
            cout << "pred : ";
            for (const auto& node : nodes[i]->predecessor)
                cout << node + 1 << ",";
            cout << endl;

            cout << "succ : ";
            for (const auto& node : nodes[i]->successor)
                cout << node + 1 << ",";
            cout << endl;

            cout << "use : ";
            for (const auto& node : nodes[i]->use)
                cout << node << ",";
            cout << endl;

            cout << "define : ";
            for (const auto& node : nodes[i]->def)
                cout << node << ",";
            cout << endl;
        }
    }

    void print_inout() {
        using namespace std;
        for (size_t i = 0; i < nodes.size(); i++) {
            cout << "node" << i + 1 << endl;
            cout << "int : ";
            for (const auto& node : nodes[i]->in)
                cout << node << ",";
            cout << endl;

            cout << "out : ";
            for (const auto& node : nodes[i]->out)
                cout << node << ",";
            cout << endl;
        }
    }

    // 最重要的函数 活跃性计算
    void liveness_analysis() {
        bool is_done = false;
        // std::set<symbol_type> difference{};
        size_t current_size;
        std::vector<symbol_type> difference;
        node_ptr successor_node_ptr = nullptr;
        node_ptr current_node_ptr = nullptr;
        for (const auto& node : nodes) {
            node->in.clear();
            node->out.clear();
        }
        // 只要没有计算完毕就一直计算
        while (!is_done) {
            is_done = true;
            // 哦没法逆序遍历
            // 逆序遍历
            for (auto node = nodes.rbegin(); node != nodes.rend(); node++) {
                current_node_ptr = *node;

                // 先算out
                // 找到node的所有后继结点
                for (const auto& successor_node : (**node).successor) {
                    // 将所有后继结点的successor_node.in -> node.out
                    // int syccessor_node
                    successor_node_ptr = nodes[successor_node];

                    current_size = current_node_ptr->out.size();
                    current_node_ptr->out.insert(successor_node_ptr->in.begin(),
                                                 successor_node_ptr->in.end());
                    if (current_node_ptr->out.size() > current_size) {
                        // 说明添加了新的元素
                        is_done = false;
                    }
                }

                // 然后计算in
                // in[n] = use[n] + (out[n]-def[n])
                current_size = current_node_ptr->in.size();
                current_node_ptr->in.insert(current_node_ptr->use.begin(),
                                            current_node_ptr->use.end());
                // 计算 out-def

                difference.clear();
                difference.resize(current_node_ptr->out.size() +
                                  current_node_ptr->def.size());
                auto first = difference.begin();
                const auto last = std::set_difference(
                    current_node_ptr->out.begin(), current_node_ptr->out.end(),
                    current_node_ptr->def.begin(), current_node_ptr->def.end(),
                    difference.begin());

                current_node_ptr->in.insert(first, last);
                if (current_node_ptr->in.size() > current_size) {
                    // 说明添加了新的元素
                    is_done = false;
                }
            }
        }
    }

    using node_type = ControlFlowGraphNode;
    using node_ptr = std::shared_ptr<node_type>;

    std::vector<node_ptr> nodes;

  private:
    // make cfg
    // void find_leaders();
    // void build_graph();

    void init_cfg();

    std::unordered_map<std::string, BasicBlock> blocks;
    util::DirectedGraph<std::string> cfg;
    InstructionList _insts;
    Env env;
};

std::unique_ptr<ControlFlowGraph> make_cfg(InstructionList insts);

} // namespace cgen