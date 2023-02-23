#pragma once
/*
        3.6 无作用域的符号表 内部使用unordered_map实现
        key = string
        value = value_type
*/

#ifndef _CML_UNSCOPED_TABLE_H_
#define _CML_UNSCOPED_TABLE_H_

#include "table_interface.h"
#include <string>
#include <unordered_map>

// Value必须是智能指针
template <typename ValueType>
class UnscopedTable : public TableInterface<ValueType> {
  public:
    using base_type = TableInterface<ValueType>;
    using key_type = typename base_type::key_type;
    using value_type = typename base_type::value_type;

    // 如果找到了返回
    // 如果找不到返回默认构造
    // TODO 测试符号表的时候把查找失败的返回类型改为nullptr
    virtual value_type find(const key_type& name) const override {
        // result是一个迭代器
        // *result 是 pair
        // first const key
        // second value
        auto result = table.find(name);
        if (result != table.end())
            return result->second;
        else
            return value_type{};
    }

    virtual bool insert(const key_type& name,
                        const value_type& symbol) override {
        // result是一个pair
        // first 是插入元素的迭代器 查看find
        // second bool 是插入是否成功
        auto result = table.insert({name, symbol});
        return result.second;
    }

    // 编写一个to_string 函数 可以输出当前符号表中的所有符号
    std::string to_string() const noexcept {
        std::string table_string;
        for (const auto& pair : table) {
            table_string += pair.first;
            table_string += " ";
        }
        return table_string;
    }

  private:
    std::unordered_map<key_type, value_type> table;
};

using IntTable = UnscopedTable<int>;

#endif // _CML_UNSCOPED_TABLE_H_