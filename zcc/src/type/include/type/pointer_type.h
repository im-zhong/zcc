#pragma once

/*
        3.5
*/
#ifndef _ZCC_POINTER_TYPE_H_
#define _ZCC_POINTER_TYPE_H_

#include "primitive_type.h"

#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

class PointerType : public Type {
  public:
    // 这是指针的大小 或许不应该这么草率的填写
    PointerType(const std::shared_ptr<Type>& type)
        : Type(pointer_mask, 8), value_type(type) {}

    std::shared_ptr<Type> dereference_type() const { return value_type; }
    virtual std::string to_string() const noexcept override;

  private:
    std::shared_ptr<Type> value_type;
};

std::shared_ptr<PointerType> make_pointer(const std::shared_ptr<Type>& type);
// std::shared_ptr<Type> make_dereference(std::shared_ptr<Type> pointer_type);

_CML_END_
#endif // _ZCC_POINTER_TYPE_H_