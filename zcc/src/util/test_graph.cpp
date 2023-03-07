// 2023/3/6
// zhangzhong

#include "util/graph.h"
#include <gtest/gtest.h>
#include <iostream>

void test_undirected_graph() {
    util::UndirectedGraph<std::string> graph{};
    graph.dump();
    EXPECT_FALSE(graph.has_node("node"));
    EXPECT_FALSE(graph.has_edge("from", "to"));

    graph.insert_edge("from", "to");
    graph.dump();
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_TRUE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_edge("from", "to"));
    EXPECT_TRUE(graph.has_edge("to", "from"));

    graph.insert_edge("hello", "to");
    graph.dump();
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_TRUE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_node("hello"));
    EXPECT_TRUE(graph.has_edge("from", "to"));
    EXPECT_TRUE(graph.has_edge("to", "from"));
    EXPECT_TRUE(graph.has_edge("hello", "to"));
    EXPECT_TRUE(graph.has_edge("to", "hello"));
    EXPECT_FALSE(graph.has_edge("from", "hello"));
    EXPECT_FALSE(graph.has_edge("hello", "from"));

    graph.hide_node("to");
    graph.dump();
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_FALSE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_node("hello"));
    EXPECT_FALSE(graph.has_edge("from", "to"));
    EXPECT_FALSE(graph.has_edge("to", "from"));
    EXPECT_FALSE(graph.has_edge("hello", "to"));
    EXPECT_FALSE(graph.has_edge("to", "hello"));
    EXPECT_FALSE(graph.has_edge("from", "hello"));
    EXPECT_FALSE(graph.has_edge("hello", "from"));

    // 因为上面已经把to hide了 所以从逻辑上我们是看不到to的邻接的
    const auto& adj = graph.get_adjacency("to");
    EXPECT_TRUE(adj.empty());
    const auto& adj2 = graph.get_adjacency("from");
    EXPECT_TRUE(adj2.empty());
    EXPECT_EQ(graph.get_degree("from"), 0);
    EXPECT_EQ(graph.get_degree("to"), -1);
    EXPECT_EQ(graph.get_degree("hello"), 0);

    graph.insert_node("to");
    graph.dump();
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_TRUE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_node("hello"));
    EXPECT_TRUE(graph.has_edge("from", "to"));
    EXPECT_TRUE(graph.has_edge("to", "from"));
    EXPECT_TRUE(graph.has_edge("hello", "to"));
    EXPECT_TRUE(graph.has_edge("to", "hello"));
    EXPECT_FALSE(graph.has_edge("from", "hello"));
    EXPECT_FALSE(graph.has_edge("hello", "from"));

    const auto& adj3 = graph.get_adjacency("to");
    EXPECT_FALSE(adj3.empty());
    EXPECT_TRUE(adj3.contains("from"));
    EXPECT_TRUE(adj3.contains("hello"));
    const auto& adj4 = graph.get_adjacency("from");
    EXPECT_FALSE(adj4.empty());
    EXPECT_TRUE(adj4.contains("to"));

    EXPECT_EQ(graph.get_degree("from"), 1);
    EXPECT_EQ(graph.get_degree("to"), 2);
    EXPECT_EQ(graph.get_degree("hello"), 1);
}

void test_directed_graph() {
    util::DirectedGraph<std::string> graph{};
    graph.dump();
    EXPECT_FALSE(graph.has_node("node"));
    EXPECT_FALSE(graph.has_edge("from", "to"));

    graph.insert_edge("from", "to");
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_TRUE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_edge("from", "to"));
    EXPECT_FALSE(graph.has_edge("to", "from"));

    graph.insert_edge("hello", "to");
    EXPECT_TRUE(graph.has_node("from"));
    EXPECT_TRUE(graph.has_node("to"));
    EXPECT_TRUE(graph.has_node("hello"));
    EXPECT_TRUE(graph.has_edge("from", "to"));
    EXPECT_FALSE(graph.has_edge("to", "from"));
    EXPECT_TRUE(graph.has_edge("hello", "to"));
    EXPECT_FALSE(graph.has_edge("to", "hello"));
    EXPECT_FALSE(graph.has_edge("from", "hello"));
    EXPECT_FALSE(graph.has_edge("hello", "from"));
}

int main(int argc, char* argv[]) {
    test_undirected_graph();
    test_directed_graph();
}
