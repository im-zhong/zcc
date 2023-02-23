#pragma once
#ifndef _ZCC_FUNCTION_TYPE_H_
#define _ZCC_FUNCTION_TYPE_H_

#include "primitive_type.h"

#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

class FunctionType : public Type {
  public:
    FunctionType(const std::shared_ptr<Type>& return_type,
                 std::list<std::shared_ptr<Type>> parameter_list_type)
        : Type(function_mask | pointer_mask, 8), // 8 是指针的大小
          return_type(return_type),
          parameter_list_type(std::move(parameter_list_type)) {}

    std::shared_ptr<Type> get_return_type() const noexcept {
        return return_type;
    }
    const std::list<std::shared_ptr<Type>>&
    get_parameter_list_type() const noexcept {
        return parameter_list_type;
    }
    int get_parameter_size() const noexcept {
        return parameter_list_type.size();
    }
    bool is_variable_parameter() const noexcept {
        return !parameter_list_type.empty() &&
               parameter_list_type.back() == variable_type;
    }
    virtual std::string to_string() const noexcept override;

  private:
    std::shared_ptr<Type> return_type;
    std::list<std::shared_ptr<Type>> parameter_list_type;
};

std::shared_ptr<FunctionType>
make_function(const std::shared_ptr<Type>& return_type,
              const std::list<std::shared_ptr<Type>>& parameter_list_type);

// std::shared_ptr<PointerType>
// make_function_decay(std::shared_ptr<FunctionType> function_type);
_CML_END_
#endif // _ZCC_FUNCTION_TYPE_H_