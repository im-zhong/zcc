// 2023/3/5
// zhangzhong

#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace cgen {
class ControlFlowGraph {
  public:
    using size_type = std::size_t;
    using symbol_type = std::string;

    struct ControlFlowGraphNode {
        std::set<size_type> predecessor;
        std::set<size_type> successor;

        std::set<symbol_type> define;
        std::set<symbol_type> use;

        std::set<symbol_type> in;
        std::set<symbol_type> out;
    };

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

        node1->define.insert({"a"});
        node2->use.insert({"a"});
        node2->define.insert({"b"});
        node3->use.insert({"b", "c"});
        node3->define.insert({"c"});
        node4->use.insert({"b"});
        node4->define.insert({"a"});
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
            for (const auto& node : nodes[i]->define)
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
                                  current_node_ptr->define.size());
                auto first = difference.begin();
                const auto last = std::set_difference(
                    current_node_ptr->out.begin(), current_node_ptr->out.end(),
                    current_node_ptr->define.begin(),
                    current_node_ptr->define.end(), difference.begin());

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
};
} // namespace cgen