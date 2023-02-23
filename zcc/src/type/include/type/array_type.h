#pragma once
/*
        3.5 23:20

*/

#ifndef _ZCC_ARRAY_TYPE_H_
#define _ZCC_ARRAY_TYPE_H_

#include "primitive_type.h"
#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

class ArrayType : public Type {
  public:
    ArrayType(const std::shared_ptr<Type>& type, size_t element_size)
        : Type(array_mask | pointer_mask, element_size * type->get_size()),
          element_type(type), element_size(element_size) {}

    std::shared_ptr<Type> get_element_type() const noexcept {
        return element_type;
    }
    int get_element_size() const noexcept { return element_size; }
    virtual std::string to_string() const noexcept override;

  private:
    std::shared_ptr<Type> element_type;
    int element_size;
};

std::shared_ptr<ArrayType> make_array(const std::shared_ptr<Type>& type,
                                      size_t element_count);

// std::shared_ptr<PointerType> make_array_decay(std::shared_ptr<Type>
// array_type);

_CML_END_

#endif // _ZCC_ARRAY_TYPE_H_