#pragma once
/*
        检查类型
*/

#ifndef _ZCC_TYPE_ASSERT_H_
#define _ZCC_TYPE_ASSERT_H_

#include "type_metric.h"
_CML_BEGIN_
//
// 提供修改功能 类似这样的
constexpr void set_int(uint32_t& type_word) noexcept { type_word |= int_mask; }

// 我还应该提供诊断

// native定义为没有任何操作符和修饰字 且其类型位 void ... 等七种类型的一种
// 获得类型的前缀 也就是 op_q,op,q,0
// auto prefix = ;
// 如果prefix是0 那么类型就是native的
// bool is_native(uint32_t type_word) { return !static_cast<bool>(type_word &
// ~type_mask); }

// 这些函数都实现在了type 内部
// 他们实际上应该对类型字进行检测 这样可以使上层完全脱离枚举值 枚举怎么变都可以
constexpr bool is_unsigned(uint32_t type_word) noexcept {
    return type_word & unsigned_mask;
}
constexpr bool is_const(uint32_t type_word) noexcept {
    return type_word & const_mask;
}
constexpr bool is_volatile(uint32_t type_word) noexcept {
    return type_word & volatile_mask;
}
constexpr bool is_const_volatile(uint32_t type_word) noexcept {
    return is_const(type_word) && is_volatile(type_word);
}

constexpr bool is_const_operator(uint32_t type_word) noexcept {
    return type_word & const_operator_mask;
}
constexpr bool is_volatile_operator(uint32_t type_word) noexcept {
    return type_word & volatile_operator_mask;
}
constexpr bool is_const_volatile_operator(uint32_t type_word) noexcept {
    return is_const_volatile(type_word) && is_volatile_operator(type_word);
}

constexpr bool is_array(uint32_t type_word) noexcept {
    return type_word & array_mask;
}
constexpr bool is_struct(uint32_t type_word) noexcept {
    return type_word & struct_mask;
}
constexpr bool is_union(uint32_t type_word) noexcept {
    return type_word & union_mask;
}
constexpr bool is_enum(uint32_t type_word) noexcept {
    return type_word & enum_mask;
}
constexpr bool is_function(uint32_t type_word) noexcept {
    return type_word & function_mask;
}
constexpr bool is_pointer(uint32_t type_word) noexcept {
    return type_word & pointer_mask;
}

// 关于类型的判断应该还要再稍微复杂一些
// 如果一个类型是 int 那么他的typeword 应该仅仅包含int 而已 d
// 但是像这样简单的判断就会导致任何int的变种都是int 不管是 int* 还是 int[10]
// 这种东西
constexpr bool is_void(uint32_t type_word) noexcept {
    return type_word & void_mask;
}
constexpr bool is_char(uint32_t type_word) noexcept {
    return type_word & char_mask;
}
constexpr bool is_short(uint32_t type_word) noexcept {
    return type_word & short_mask;
}
constexpr bool is_int(uint32_t type_word) noexcept {
    return type_word & int_mask;
}
constexpr bool is_long(uint32_t type_word) noexcept {
    return type_word & long_mask;
}
constexpr bool is_float(uint32_t type_word) noexcept {
    return type_word & float_mask;
}
constexpr bool is_double(uint32_t type_word) noexcept {
    return type_word & double_mask;
}

constexpr bool is_integral(uint32_t type_word) noexcept {
    return is_char(type_word) || is_short(type_word) || is_int(type_word) ||
           is_long(type_word) || is_enum(type_word);
}

constexpr bool is_floating(uint32_t type_word) noexcept {
    return is_float(type_word) || is_double(type_word);
}

constexpr bool is_arithmetic(uint32_t type_word) noexcept {
    return is_integral(type_word) || is_floating(type_word);
}

constexpr bool is_scalar(uint32_t type_word) noexcept {
    return is_arithmetic(type_word) || is_enum(type_word);
}

constexpr bool is_primitive(uint32_t type_word) noexcept {
    return is_integral(type_word) || is_floating(type_word) ||
           is_void(type_word);
}

constexpr bool is_aggregate(uint32_t type_word) noexcept {
    return is_array(type_word) || is_struct(type_word) || is_union(type_word);
}

// get

constexpr int get_operator(uint32_t type_word) noexcept {
    return (0x0000ffffu) & (type_word >> operator_lsh);
}

constexpr int get_type(uint32_t type_word) noexcept {
    return 0x0000ffffu & (type_word >> type_lsh);
}

// clear
// 注意 clear 仅清空对应的8bit
// 如果你想清空type的同时清空qualifier 需要调用两次
constexpr void clear_qualifier(uint32_t& type_word) noexcept {
    type_word &= ~qualifier_mask;
}

constexpr void clear_type(uint32_t& type_word) noexcept {
    type_word &= ~(type_mask);
}

constexpr void clear_operator_qualifier(uint32_t& type_word) noexcept {
    type_word &= ~operator_qualifier_mask;
}

constexpr void clear_operator(uint32_t& type_word) noexcept {
    type_word &= ~(operator_mask);
}

// set
// set的时候可以对原来的位清空 在用 |= 赋值
// 也可以不清空 直接赋值
// 此处函数的实现是清空
// 以后视具体情况可以改变
// 注意 set 仅设置对应的8bit
constexpr void set_qualifier(uint32_t& type_word, uint32_t qualifier) {
    clear_qualifier(type_word);
    type_word |= (qualifier << qualifier_lsh);
}

// 清空
constexpr void set_type(uint32_t& type_word, uint32_t type) {
    clear_type(type_word);
    type_word |= (type << type_lsh);
}

constexpr void set_operator_qualifier(uint32_t& type_word,
                                      uint32_t operator_qualifier) {
    clear_operator_qualifier(type_word);
    type_word |= (operator_qualifier << operator_qualifier_lsh);
}

constexpr void set_operator(uint32_t& type_word, uint32_t op) {
    clear_operator(type_word);
    type_word |= (op << operator_lsh);
}

_CML_END_
#endif // _ZCC_TYPE_ASSERT_H_
