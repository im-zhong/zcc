

#include "type/type_check.h"
#include "type/type.h"

_CML_BEGIN_

// 必须保证所有的类型仅有一份副本
bool is_same(const std::shared_ptr<Type>& lhs,
             const std::shared_ptr<Type>& rhs) noexcept {
    // lhs.get() == rhs.get()这种写法比较保险 但是直接比较 lhs == rhs
    // 应该是一样的
    return lhs && rhs && lhs == rhs; //
}

bool type_check(const std::shared_ptr<Type>& lhs,
                const std::shared_ptr<Type> rhs) noexcept {
    if (lhs == rhs)
        return true;
    if ((lhs->type_word & rhs->type_word) == 0)
        return false;
    // 然后根据不同的op进行选择
    auto type_word = lhs->type_word;
    if (is_array(type_word)) {
        auto array_lhs = std::dynamic_pointer_cast<ArrayType>(lhs);
        auto array_rhs = std::dynamic_pointer_cast<ArrayType>(rhs);
        return lhs->size != 0 && rhs->size != 0 && lhs->size == rhs->size &&
               is_same(array_lhs->get_element_type(),
                       array_rhs->get_element_type());
    }
    if (is_pointer(type_word)) {
        auto pointer_lhs = std::dynamic_pointer_cast<PointerType>(lhs);
        auto pointer_rhs = std::dynamic_pointer_cast<PointerType>(rhs);
        return is_same(pointer_lhs->dereference_type(),
                       pointer_rhs->dereference_type());
    }
    if (is_function(type_word)) {
        auto function_lhs = std::dynamic_pointer_cast<FunctionType>(lhs);
        auto function_rhs = std::dynamic_pointer_cast<FunctionType>(rhs);
        if (is_same(function_lhs->get_return_type(),
                    function_rhs->get_return_type())) {
            // 判断函数的参数列表是否相同
            // 1. 有一个是variable paramter 的都不相等
            if (function_lhs->is_variable_parameter() ||
                function_rhs->is_variable_parameter())
                return false;
            if (function_lhs->get_parameter_size() !=
                function_rhs->get_parameter_size())
                return false;
            // 如果参数列表都不是可变的 而且数目也相同
            // 那么参数列表中的类型必须完全一致才相同
            const auto& lhs_parameter_list =
                function_lhs->get_parameter_list_type();
            const auto& rhs_parameter_list =
                function_rhs->get_parameter_list_type();
            return lhs_parameter_list ==
                   rhs_parameter_list; // 容器应该提供了相等性检测吧
        } else
            return false;
    }

    // struct enum union 只有同一个类型才相等
    return false;
}

std::shared_ptr<Type> type_promote(const std::shared_ptr<Type>& type) noexcept {
    // if (type.is_primitive())
    //{
    //	if (type.is_integral())
    //		return int_type;
    //	else if (type.is_floating())
    //		return double_type;
    // }
    if (is_primitive(type->type_word)) {
        if (is_integral(type->type_word))
            return int_type;
        else if (is_floating(type->type_word))
            return double_type;
    }
    return type;
}
_CML_END_
