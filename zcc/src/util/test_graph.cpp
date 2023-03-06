// 2023/3/6
// zhangzhong

#include "util/graph.h"
#include <gtest/gtest.h>
#include <iostream>

void test_undirected_graph() {
    util::UndirectedGraph<std::string, 8> graph{};
    // 它不会输出化 兄弟们
    graph.dump();

    EXPECT_FALSE(graph.has_node("node"));
    EXPECT_FALSE(graph.has_edge("from", "to"));

    graph.insert_edge("from", "to");
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
}

int main(int argc, char* argv[]) { test_undirected_graph(); }
