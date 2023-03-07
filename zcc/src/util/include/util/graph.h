// 2023/3/5
// zhangzhong

#pragma once

#include <algorithm>
#include <array>
#include <bits/ranges_algobase.h>
#include <bits/ranges_util.h>
#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace util {

// 无权有向图
template <typename Node> class DirectedGraph {
  public:
    using node_type = Node;

    void dump() {
        // 输出所有的节点 以及所有的边
        for (const auto& node : graph) {
            std::cout << node.first << ": ";
            for (const auto& adj : node.second) {
                std::cout << node.first << "->" << adj << ", ";
            }
            std::cout << std::endl;
        }
    }

    bool has_node(Node node) { return graph.find(node) != graph.end(); }

    bool has_edge(Node from, Node to) {
        if (!has_node(from) || !has_node(to)) {
            return false;
        }
        auto& adj = graph.at(from);
        return std::ranges::find(adj, to) != adj.end();
    }

    void insert_node(Node node) {
        if (has_node(node)) {
            return;
        }
        graph.insert({node, {}});
    }

    void insert_edge(Node from, Node to) {
        // 被插入的两个节点都需要存在
        insert_node(from);
        insert_node(to);
        auto& adj = graph.at(from);
        adj.push_back(to);
    }

    // 得到某个节点的邻接节点
    std::list<node_type> get_adjacency(Node node) {
        if (has_node(node)) {
            return graph.at(node);
        } else {
            return {};
        }
    }

  private:
    std::unordered_map<Node, std::list<node_type>> graph;

    // 还得区分是否存在节点

    size_t id = 0;
    size_t get_id() { return id++; }
};

using std::size_t;

// 无权无向图
template <typename Key> class UndirectedGraph {
  public:
    using node_type = Key;
    UndirectedGraph() {
        // 初始化所有的数组
        // std::ranges::fill(matrix, false);
    }

    void insert_node(Key key) {
        // 首先我们就得从hashmap中查找了
        // if (nodes.find(key) == nodes.end()) {
        //     // 没找到, 我们需要一个新的节点
        //     nodes.insert({key, Node{.is_hide = false}});
        // } else {
        //     // 如果找到了 但是hide了 那么重新显现
        //     nodes[key].is_hide = true;
        // }
        nodes[key].is_hide = false;
    }

    // auto begin() {

    //  }
    // auto end() { return nodes.end(); }

    // bool empty() {

    // }

    std::set<Key> get_nodes() {
        auto unhided_nodes = std::set<Key>{};
        for (const auto& [key, node] : nodes) {
            if (!node.is_hide) {
                unhided_nodes.insert(key);
            }
        }
        return unhided_nodes;
    }

    void erase_node(Key key) {
        if (auto it = nodes.find(key); it != nodes.end()) {
            // 找到了 才需要删除
            // erase_node(it->second);
            nodes.erase(it);

            // todo 如果成功删除了点 相应的边需要删除
            // 但是这个可能不需要写代码 就能实现

            // 然后删除这个节点所有的边
            graph.erase(key);
            // 然后遍历所有图的所有边 将他们删掉
            for (const auto& [key, adjs] : graph) {
                adjs.erase(key);
            }
        }
    }

    void insert_edge(Key from, Key to) {
        // 首先还是找到这个两个节点对应的索引
        // 不对 插入边的时候也会插入相应的节点 是不是非常的方便呢
        // if (auto it = nodes.find(from); it == nodes.end()) {
        //     insert_node(from);
        // }
        // if (auto it = nodes.find(to); it == nodes.end()) {
        //     insert_node(to);
        // }

        insert_node(from);
        insert_node(to);
        auto from_it = nodes.find(from);
        auto to_it = nodes.find(to);
        assert(from_it != nodes.end());
        assert(to_it != nodes.end());

        // insert_edge_to_matrix(from_it->second, to_it->second);
        graph[from].insert(to);
        graph[to].insert(from);
    }

    void erase_edge(Key from, Key to) {

        // 如果一个节点被hide了 那么它能够被删除吗??

        auto from_it = nodes.find(from);
        auto to_it = nodes.find(to);

        // 只要有一个不存在就不应该删
        if (from_it == nodes.end() || to_it == nodes.end()) {
            return;
        }
        // erase_edge(from_it->second, to_it->second);
        graph[from].erase(to);
        graph[to].erase(from);
    }

    bool has_edge(Key from, Key to) {

        // has_edge 时需要考虑hide
        if (!has_node(from) || !has_node(to)) {
            return false;
        }

        auto from_it = nodes.find(from);
        auto to_it = nodes.find(to);

        // if (from_it == nodes.end() || to_it == nodes.end()) {
        //     return false;
        // }
        // return has_edge(from_it->second, to_it->second);

        bool r1 = graph[from].contains(to);
        bool r2 = graph[to].contains(from);
        assert(r1 == r2);
        return r1;
    }

    bool has_node(Key key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) {
            return false;
        }
        if (it->second.is_hide) {
            return false;
        }
        return true;
    }

    void dump() {
        // 输出所有true的节点
        // {0->1, }
        // {0: hello, 1: xxx,}

        // for (auto [key, id] : nodes) {
        //     std::cout << key << ": " << id << ", ";
        // }
        // std::cout << std::endl;

        // for (size_t i = 0; i < N; i++) {
        //     for (size_t j = 0; j < N; j++) {
        //         if (*index(i, j)) {
        //             std::cout << i << "->" << j << ", ";
        //         }
        //     }
        // }
        // std::cout << std::endl;

        for (const auto& [key, node] : nodes) {
            std::cout << key << ":" << node.is_hide << ", ";
        }
        std::cout << std::endl;

        // 然后是输出所有的边
        for (const auto& [key, adjs] : graph) {
            std::cout << key << " -> ";
            for (const auto& adj : adjs) {
                std::cout << adj << ", ";
            }
            std::cout << std::endl;
        }
    }

    std::set<Key> get_adjacency(Key key) {
        auto adjs = std::set<Key>{};
        if (has_node(key)) {
            for (const auto& adj : graph[key]) {
                if (!is_hide(adj)) {
                    adjs.insert(adj);
                }
            }
        }
        return adjs;
    }

    // 提供一个hide接口 就是不真正的删除某个节点 只是隐藏起来

    void hide_node(Key key) {
        if (nodes.find(key) != nodes.end()) {
            nodes[key].is_hide = true;
        }
    }

    // 被hide的节点重新插入之后就会唤醒相关的边

    bool is_hide(Key key) {
        // 查看某个节点是否被隐藏了
        if (nodes.find(key) != nodes.end()) {
            return nodes[key].is_hide;
        }

        return false;
    }

    int get_degree(Key key) {
        // 获得某个节点的degree
        if (!has_node(key)) {
            return -1;
        }

        // 有这个节点
        // int degree = 0;
        return get_adjacency(key).size();
    }

    // 然后就是degree
    // 返回某个节点的degree

    // 这个东西也得实现一个邻接链表
    // 这个图没有办法通用
    // 他就专门给冲突图使用吧

  private:
    // bool erase_node(size_t i) {
    //     // 有两种方法实现这个东西
    //     // 1. 提供一个node数组 指出现在某些节点是存在还是删除
    //     assert(i < N);
    //     nodes[i] = false;

    //     // 然后把graph里面该该节点有关的边都删了
    // }

    // void erase_edge(size_t from, size_t to) {
    //     assert(from < N);
    //     assert(to < N);
    //     *index(from, to) = false;
    //     *index(to, from) = false;
    // }

    // void insert_edge_to_list() {}

    // void insert_edge_to_matrix(size_t from, size_t to) {
    //     assert(from < N);
    //     assert(to < N);
    //     *index(from, to) = true;
    //     *index(to, from) = true;
    // }

    // bool has_edge(size_t from, size_t to) {
    //     assert(from < N);
    //     assert(to < N);
    //     assert(*index(from, to) == *index(to, from));
    //     return *index(from, to);
    // }

    // // void has_node(size_t i) {
    // //     assert(i < N);
    // //     return nodes[i];
    // // }

    // bool* index(std::size_t i, std::size_t j) {
    //     assert(i < N);
    //     assert(j < N);
    //     return &matrix[i * N + j];
    // }

  private:
    // 二维数组
    // std::array<bool, N * N> matrix;

    struct Node {
        // size_t id;
        bool is_hide;
    };

    // 这个数组现在用不到了 下面的mapping就提供了这样的功能了
    // std::array<bool, N> nodes;
    std::unordered_map<Key, Node> nodes;

    // std::unordered_map<Key, Node> mapping2;

    // 这个东西可以用vector来保存
    // 那这样的话 全都用这个玩意来实现不就行了吗
    std::unordered_map<Key, std::set<Key>> graph;

    // std::vector<std::list<Key>> list;
    // 但是这个东西的大小是固定的 那没法用了
    // 用这个玩意来表示对称矩阵
    // 当增加节点超过matrix可以表示的最大节点时，需要重新分配节点
    // 这个开销非常大呀 这些优化之后再向吧
    // std::vector<bool> matrix;
    // std::vector<Node> nodes;

    // size_t id = 0;
    // // 我们还需要一个链表用来管理被删除的节点
    // // 这样可以重新分配那些被删除的节点
    // // 或者我们可以简单的使用一个递增器
    // size_t get_id() {
    //     assert(id < N);
    //     return id++;
    // }
};

} // namespace util