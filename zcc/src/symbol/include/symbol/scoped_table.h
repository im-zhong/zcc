#pragma once
/*
        3.6 有作用域的符号表 内部使用unordered_map实现
        key = string
        value = value_type
*/
/*
        最重要的符号表

        分析 : 符号表分两种 一种是涉及到作用域管理的 identifier types external
                                           另一种是没有涉及到作用域管理的 global
   constants labels


        而这些符号表做的事情只不过就是 插入 查找 删除 遍历 更改？
   我可以修改变量的值啊

        符号表也应该遵从继承体系
        Symbol {name, scope, coor, storage_type, type}
        LabelSymbol : Symbol
        StructSymbol : Symbol
        EnumSymbol: Symbol
        ConstantSymbol : Symbol
        FunctionSymbol : Symbol
        GlobalSymbol : Symbol
        TemporarySymbol : Symbol

        对应的符号表保存对应的值

        TODO 3.2
        我先实现Type类型 然后根据这个类型实现 不涉及作用域管理的常量表
   整理一下思路

        interface			inte
        implementation		impl
        dispatcher			disp

*/

#ifndef _CML_SCOPED_TABLE_H_
#define _CML_SCOPED_TABLE_H_

#include "table_interface.h"
#include "unscoped_table.h"
#include <forward_list>
#include <memory>
#include <string>
#include <unordered_map>

template <typename ValueType>
class TableEntry : public UnscopedTable<ValueType> {
  public:
    using base_type = UnscopedTable<ValueType>;
    using key_type = typename base_type::key_type;
    using value_type = typename base_type::value_type;

    TableEntry(int scope) : scope(scope) {}
    constexpr int get_scope() const noexcept { return scope; }
    std::string to_string() const noexcept {
        std::string table_string;
        table_string += "scope ";
        table_string += std::to_string(scope);
        table_string += ": ";
        table_string += base_type::to_string();
        return table_string;
    }

  private:
    int scope;
};

template <typename ValueType>
class ScopedTable : public TableInterface<ValueType> {
  public:
    using entry_type = TableEntry<ValueType>;
    using base_type = UnscopedTable<ValueType>;
    using key_type = typename base_type::key_type;
    using value_type = typename base_type::value_type;

    // 在当前表中添加一个符号
    // 我肯定是分给下属做啊
    bool insert(const key_type& name, const value_type& symbol) override {
        // 在插入符号的时候有可能会导致创建一个新的entry
        // 那就是 symbol->get_scope == current_scope
        if (current_scope > entry_scope) {
            // 就说明我需要创建一个新的entry
            // 并且更新 entry_scope
            make_entry();
            entry_scope = current_scope;
        }
        auto& entry = table.front();
        return entry->insert(name, symbol);
    }

    // TODO 当使用智能指针做测试时 把空返回的{}改为nullptr
    value_type find(const key_type& name) const override {
        // 从当前作用域开始 一层一层的往上找
        value_type symbol{};
        for (const auto& entry : table) {
            symbol = entry->find("name");
            if (symbol)
                return symbol;
        }
        return symbol;
    }

    std::string to_string() const noexcept {
        std::string table_string;
        table_string += "current scope " + std::to_string(current_scope) + "\n";
        table_string += "entry scope " + std::to_string(entry_scope) + "\n";
        for (const auto& entry : table) {
            table_string += entry->to_string();
            table_string += "\n";
        }
        return table_string;
    }

    void enter_scope() noexcept { ++current_scope; }

    void leave_scope() noexcept {
        // 退出作用域 但是需要检查在本作用域内是否定义了新的符号添加到了符号表
        // 其实就是检查当前作用域和创建的entry的最大的作用域是否相等
        auto front_entry = get_front_entry();
        if (front_entry && current_scope == front_entry->get_scope()) {
            // 那么删除顶层符号入口
            table.pop_front();
            // 并且更新 entry_scope
            entry_scope = get_entry_scope_impl();
        }
        --current_scope;
    }

  private:
    int get_current_scope() const noexcept { return current_scope; }

    // 返回现在最靠前的entry的scope
    // 如果没有entry 返回-1
    int get_entry_scope_impl() const noexcept {
        auto entry = get_front_entry();
        if (entry)
            return entry->get_scope();
        else
            return -1;
    }

    int get_entry_scope() const noexcept { return entry_scope; }

    // 创建一个新的符号表入口点 一般来说需要进入一个新的作用域
    // make_table_entry
    std::shared_ptr<entry_type> make_entry() {
        table.emplace_front(std::make_shared<entry_type>(current_scope));
        return table.front();
    }

    std::shared_ptr<entry_type> get_front_entry() const noexcept {
        if (table.empty())
            return nullptr;
        else
            return table.front();
    }

  private:
    int current_scope{};
    int entry_scope{-1};
    std::forward_list<std::shared_ptr<entry_type>> table;
};

#endif // _CML_SCOPED_TABLE_H_