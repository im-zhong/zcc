#pragma once
#ifndef _ZCC_ENUM_TYPE_H_
#define _ZCC_ENUM_TYPE_H_

#include "primitive_type.h"
#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

class EnumType : Type {
  public:
    // Enum 的域也是简单的符号域 并且这些符号会加载到 identifier 表中
    EnumType(const std::string& enum_name,
             const std::list<std::shared_ptr<Symbol>>& enum_constants)
        : Type(union_mask, get_underlying_type()->get_size(),
               get_underlying_type()->get_align()) {
        // TODO
        // 把这些枚举常量 enum constatns 添加到符号表中
    }

    std::string get_name() const noexcept { return name; }
    static std::shared_ptr<Type> get_underlying_type() noexcept {
        return int_type;
    }
    virtual std::string to_string() const noexcept override;

  private:
    std::string name;
};

std::shared_ptr<EnumType>
make_enum(const std::string& enum_name,
          const std::list<std::shared_ptr<Symbol>>& enum_constants);

_CML_END_

#endif // _ZCC_ENUM_TYPE_H_