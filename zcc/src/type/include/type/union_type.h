#pragma once
#ifndef _ZCC_UNION_TYPE_H_
#define _ZCC_UNION_TYPE_H_

#include "aggregate_type.h"
#include "primitive_type.h"
#include "symbol/symbol.h"
#include "type_metric.h"
#include <algorithm>
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_
class UnionType : public AggregateType {
  public:
    UnionType(const std::string& union_name, bool is_define)
        : AggregateType(union_mask, union_name) {
        check_definition(union_name);
    }

    UnionType(const std::string& union_name,
              const std::list<std::shared_ptr<Symbol>>& field_symbols,
              bool is_define)
        : AggregateType(union_mask, union_name) {
        // 关于这个函数的调用 我感觉有一个问题
        //
        check_definition(union_name);
        insert(field_symbols);
    }

    virtual bool
    insert(const std::list<std::shared_ptr<Symbol>>& field_symbols) override {
        for (const auto& symbol : field_symbols) {
            if (!this->insert(field_symbols))
                return false;
        }
        return true;
    }

    virtual bool insert(const std::shared_ptr<Symbol>& symbol) override {
        // 同上
        if (find(symbol->name) == nullptr) {
            // 一开始 offset = size
            // insert
            // 插入结束之后 size += offset
            fields.emplace_back(make_field(symbol, 0));
            size = std::max(size, symbol->get_size());

            // 2023/2/23: maybe fix
            return true;
        } else
            return false;
    }

    virtual std::string to_string() const noexcept override;

  protected:
    // TODO 同struct
    virtual bool
    check_definition(const std::string& name) const noexcept override {
        return true;
    }
};

std::shared_ptr<UnionType> make_union(const std::string& union_name,
                                      bool is_define);

std::shared_ptr<UnionType>
make_union(const std::string& union_name,
           const std::list<std::shared_ptr<Symbol>>& field_symbols,
           bool is_define);

// 如果我想创建一个无名union那么肯定是一次定义
std::shared_ptr<UnionType> make_union();
std::shared_ptr<UnionType>
make_union(const std::list<std::shared_ptr<Symbol>>& field_symbols);
_CML_END_
#endif // _ZCC_UNION_TYPE_H_