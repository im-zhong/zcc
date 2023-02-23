/*
        2020.3.25
        tree
        dag

        2020/4/25 15：23
        TODO  优化树的实现 很明显我需要从一个节点获得对应的树
   这样非常方便而且非常应该
*/

#ifndef _ZCC_TREE_H_
#define _ZCC_TREE_H_

#include "core/core.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

_CML_BEGIN_

template <typename ValueType> class binary_tree_node;

template <typename ValueType>
std::shared_ptr<binary_tree_node<ValueType>>
make_binary_tree_node(const ValueType& value);

template <typename ValueType> class binary_tree;

// 用智能指针只要出现循环引用就会导致内存泄漏
template <typename ValueType>
class binary_tree_node
    : public std::enable_shared_from_this<binary_tree_node<ValueType>> {
    friend class binary_tree<ValueType>;

  public:
    using value_type = ValueType;
    using node_type = std::shared_ptr<binary_tree_node>;
    using weak_node_type = std::weak_ptr<binary_tree_node>;
    using tree_type = std::shared_ptr<binary_tree<value_type>>;

    binary_tree_node(const ValueType& value)
        : value(value), left(nullptr), right(nullptr), parent() {}

    binary_tree_node(const ValueType& value, weak_node_type parent)
        : value(value), left(nullptr), right(nullptr), parent(parent) {}

    node_type insert_left(const value_type& value) {
        return left = make_binary_tree_node(value, this->weak_from_this());
    }

    node_type insert_right(const value_type& value) {
        return right = make_binary_tree_node(value, this->weak_from_this());
    }

    node_type insert_left(node_type node) {
        left = node;

        // cout << left->value << endl;

        left->parent = this->weak_from_this();

        return left;
    }

    node_type insert_right(node_type node) {
        right = node;
        right->parent = this->weak_from_this();
        return right;
    }

    // 在更加方便的情况下我们可以插入一颗子树  实现起来也很方便 暂时不实现
    // 等需要的时候再实现
    // node_type insert_left(tree_type left)
    //{
    //	return insert_left(left->get_root());
    //}

    // get
    value_type& get_value() noexcept { return value; }
    node_type get_left() const noexcept { return left; }
    node_type get_right() const noexcept { return right; }
    node_type get_parent() const noexcept { return parent.lock(); }
    // set
    void set_value(const value_type& value) { this->value = value; }

    // 从跟节点获得相应的树
    // std::shared_ptr<binary_tree<ValueType>> get_tree() const
    //{

    //}

  protected:
    ValueType value;
    // 这是不能存在父节点的
    // 不然就会导致子节点掌握父节点 父节点掌握子节点 导致内存泄漏
    // std::weak_ptr<binary_tree_node> parent;
    // 那么我用弱指针不就解决这个问题了吗 刺激啊
    // 这么些可太容易出错了 啊哈哈
    // 2020/5/17 提出我需要父指针！
    weak_node_type parent;
    node_type left;
    node_type right;
};

template <typename ValueType>
std::shared_ptr<binary_tree_node<ValueType>>
make_binary_tree_node(const ValueType& value) {
    return std::make_shared<binary_tree_node<ValueType>>(value);
}

template <typename ValueType>
std::shared_ptr<binary_tree_node<ValueType>>
make_binary_tree_node(const ValueType& value,
                      std::weak_ptr<binary_tree_node<ValueType>> parent) {
    return std::make_shared<binary_tree_node<ValueType>>(value, parent);
}

// 这个模板写的不对啊
// 更普适的情况应该是
// template <typename NodeType>
template <typename ValueType> class binary_tree {
  public:
    using value_type = ValueType;
    using node_type = typename binary_tree_node<value_type>::node_type;

    binary_tree() : root(nullptr), traverse() {}

    binary_tree(const value_type& value)
        : root(make_binary_tree_node(value)), traverse() {}

    // 实现三种遍历
    // 我可以把它变成一个vector序列 这样方便操作
    // 只需要把节点都拿过来组成一个序列就行了
    const std::vector<node_type>& preorder() const {
        traverse.clear();
        if (root != nullptr)
            preorder_traverse(root);
        return traverse;
    }

    const std::vector<node_type>& inorder() const {
        traverse.clear();
        if (root != nullptr)
            inorder_traverse(root);
        return traverse;
    }

    const std::vector<node_type>& postorder() const {
        traverse.clear();
        if (root != nullptr)
            postorder_traverse(root);
        return traverse;
    }

    node_type get_root() const noexcept { return root; }

    node_type set_root(node_type root) { return this->root = root; }

    void clear() noexcept {
        traverse.clear();
        root = nullptr;
    }

    ~binary_tree() noexcept { clear(); }

  private:
    void preorder_traverse(node_type root) const {
        traverse.push_back(root);
        if (root->left)
            preorder_traverse(root->left);
        if (root->right)
            preorder_traverse(root->right);
    }

    void inorder_traverse(node_type root) const {
        if (root->left)
            inorder_traverse(root->left);
        traverse.push_back(root);
        if (root->right)
            inorder_traverse(root->right);
    }

    void postorder_traverse(node_type root) const {
        if (root->left)
            postorder_traverse(root->left);
        if (root->right)
            postorder_traverse(root->right);
        traverse.push_back(root);
    }

  private:
    node_type root;

    // 这个数据不应该由每一个节点持有吧 好像
    // 他应该由树结构来持有
    mutable std::vector<node_type> traverse;
};

template <typename ValueType>
std::shared_ptr<binary_tree<ValueType>> make_tree() {
    return std::make_shared<binary_tree<ValueType>>();
}

template <typename ValueType>
std::shared_ptr<binary_tree<ValueType>> make_tree(const ValueType& value) {
    return std::make_shared<binary_tree<ValueType>>(value);
}

_CML_END_
#endif // _ZCC_TREE_H_
