#include "ast/tree.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cml;

int main() {
    auto tree = make_tree(string("A"));
    auto node = tree->get_root();
    node->insert_left("B")->insert_left("D");
    node = node->insert_right("C");
    node->insert_left("E");
    node = node->insert_right("F");
    node->insert_left("G");

    for (auto& node : tree->preorder()) {
        cout << node->get_value();
    }
    cout << endl;

    for (auto& node : tree->inorder()) {
        cout << node->get_value();
    }
    cout << endl;

    for (auto& node : tree->postorder()) {
        cout << node->get_value();
    }
    cout << endl;

    tree->clear();

    node = tree->set_root(make_binary_tree_node(string("+")));
    node->insert_left("i");
    node = node->insert_right("*");
    node->insert_left("j");
    node->insert_right("k");
    // 后序遍历才是对的
    // 而且我可以直接通过栈式操作把后缀操作符转换成对应的操作代码
    for (auto& node : tree->postorder()) {
        cout << node->get_value();
    }
    cout << endl;
}