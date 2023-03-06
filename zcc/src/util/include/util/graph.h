// 2023/3/5
// zhangzhong

#include <algorithm>
#include <array>
#include <bits/ranges_algobase.h>
#include <bits/ranges_util.h>
#include <cassert>
#include <iostream>
#include <list>
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
template <typename Node, std::size_t N> class UndirectedGraph {
  public:
    UndirectedGraph() {
        // 初始化所有的数组
        std::ranges::fill(graph, false);
    }

    void insert_node(Node node) {
        // 首先我们就得从hashmap中查找了
        if (mapping.find(node) == mapping.end()) {
            // 没找到, 我们需要一个新的节点
            mapping.insert({node, get_id()});
        }
    }

    void erase_node(Node node) {
        if (auto it = mapping.find(node); it != mapping.end()) {
            // 找到了 才需要删除
            erase_node(it->second);

            // todo 如果成功删除了点 相应的边需要删除
            // 但是这个可能不需要写代码 就能实现
        }
    }

    void insert_edge(Node from, Node to) {
        // 首先还是找到这个两个节点对应的索引
        // 不对 插入边的时候也会插入相应的节点 是不是非常的方便呢
        if (auto it = mapping.find(from); it == mapping.end()) {
            insert_node(from);
        }
        if (auto it = mapping.find(to); it == mapping.end()) {
            insert_node(to);
        }

        auto from_it = mapping.find(from);
        auto to_it = mapping.find(to);
        assert(from_it != mapping.end());
        assert(to_it != mapping.end());
        insert_edge(from_it->second, to_it->second);
    }

    void erase_edge(Node from, Node to) {
        auto from_it = mapping.find(from);
        auto to_it = mapping.find(to);

        // 只要有一个不存在就不应该删
        if (from_it == mapping.end() || to_it == mapping.end()) {
            return;
        }
        erase_edge(from_it->second, to_it->second);
    }

    bool has_edge(Node from, Node to) {
        auto from_it = mapping.find(from);
        auto to_it = mapping.find(to);

        if (from_it == mapping.end() || to_it == mapping.end()) {
            return false;
        }
        return has_edge(from_it->second, to_it->second);
    }

    bool has_node(Node node) { return mapping.find(node) != mapping.end(); }

    void dump() {
        // 输出所有true的节点
        // {0->1, }
        // {0: hello, 1: xxx,}

        for (auto [key, id] : mapping) {
            std::cout << key << ": " << id << ", ";
        }
        std::cout << std::endl;

        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                if (*index(i, j)) {
                    std::cout << i << "->" << j << ", ";
                }
            }
        }
        std::cout << std::endl;
    }

  private:
    // bool erase_node(size_t i) {
    //     // 有两种方法实现这个东西
    //     // 1. 提供一个node数组 指出现在某些节点是存在还是删除
    //     assert(i < N);
    //     nodes[i] = false;

    //     // 然后把graph里面该该节点有关的边都删了
    // }

    void erase_edge(size_t from, size_t to) {
        assert(from < N);
        assert(to < N);
        *index(from, to) = false;
        *index(to, from) = false;
    }

    void insert_edge(size_t from, size_t to) {
        assert(from < N);
        assert(to < N);
        *index(from, to) = true;
        *index(to, from) = true;
    }

    bool has_edge(size_t from, size_t to) {
        assert(from < N);
        assert(to < N);
        assert(*index(from, to) == *index(to, from));
        return *index(from, to);
    }

    // void has_node(size_t i) {
    //     assert(i < N);
    //     return nodes[i];
    // }

    bool* index(std::size_t i, std::size_t j) {
        assert(i < N);
        assert(j < N);
        return &graph[i * N + j];
    }

  private:
    // 二维数组
    std::array<bool, N * N> graph;

    // 这个数组现在用不到了 下面的mapping就提供了这样的功能了
    // std::array<bool, N> nodes;
    std::unordered_map<Node, size_t> mapping;

    size_t id = 0;
    // 我们还需要一个链表用来管理被删除的节点
    // 这样可以重新分配那些被删除的节点
    // 或者我们可以简单的使用一个递增器
    size_t get_id() {
        assert(id < N);
        return id++;
    }
};

} // namespace util