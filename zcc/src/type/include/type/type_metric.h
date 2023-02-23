#pragma once
/*
        type metric 指定基本类型的大小和对齐字节
*/

#ifndef _ZCC_TYPE_METRIC_H_
#define _ZCC_TYPE_METRIC_H_

#include "core/core.h"
#include <stdint.h>
_CML_BEGIN_

// 现在枚举值可以随便改 都不会影响程序的行为
// 这样低5位就代表基本类型 比较方便获取
enum class TYPE : uint8_t {
    VOID = 0b00000001,
    CHAR = 0b00000010,
    INT = 0b00000100,
    FLOAT = 0b00001000,
    DOUBLE = 0b00010000,

    SHORT = 0b00100000,
    LONG = 0b01000000,

    SIGNED = 0b00000000, // 默认的位 其实不需要设置
    UNSIGNED = 0b10000000,

    // 可变类型
    VARIABLE = 0b11111111
};

// 写上面这些好像都没有用啊 我需要的是运行期的
// 元函数需要的是一个编译期常数  但是其实 我获得的是运行期的数值
// 所以如果我这么设计 就必须使用ifelse来选择 那么万不得已的情况下
// 以常数为参数的工厂模式 就挺不错的

// 对应的枚举值 其实就是掩码 这样非常有利于判断和设置相应的位
enum class QUALIFIER : uint8_t {
    CONST = 0b00000001,
    VOLATILE = 0b00000010,
    UNSIGNED = 0b00000100,
    SIGNED = 0b00001000
};

enum class TYPE_OPERATOR : uint8_t {
    ARRAY = 0b00000001,
    POINTER = 0b00000010,
    STRUCT = 0b00000100,
    UNION = 0b00001000,
    FUNCTION = 0b00010000,
    ENUM = 0b00100000
};

enum class OPERATOR_QUALIFIER : uint8_t {
    CONST = 0b00000001,
    VOLATILE = 0b00000010,
};

// typeword最高的8位
enum class STORAGE_CLASS_QUALIFIER : uint8_t {
    AUTO = 0b10000000,
    EXTERN = 0b01000000,
    STATIC = 0b00100000,
    TYPEDEF = 0b00010000,
    REGISTER = 0b00001000
};

template <typename T> struct Metric {
    constexpr static int type = 0;
    constexpr static int align = 0;
    constexpr static int size = 0;
};

template <> struct Metric<void> {
    constexpr static int type = enum_cast(TYPE::VOID);
    constexpr static int align = 0;
    constexpr static int size = 0;
};

template <> struct Metric<char> {
    constexpr static int type = enum_cast(TYPE::CHAR);
    constexpr static int align = sizeof(char);
    constexpr static int size = sizeof(char);
};

template <> struct Metric<short> {
    constexpr static int type = enum_cast(TYPE::SHORT);
    constexpr static int align = sizeof(short);
    constexpr static int size = sizeof(short);
};

template <> struct Metric<int> {
    constexpr static int type = enum_cast(TYPE::INT);
    constexpr static int align = sizeof(int);
    constexpr static int size = sizeof(int);
};

template <> struct Metric<long> {
    constexpr static int type = enum_cast(TYPE::LONG);
    constexpr static int align = sizeof(long);
    constexpr static int size = sizeof(long);
};

template <> struct Metric<float> {
    constexpr static int type = enum_cast(TYPE::FLOAT);
    constexpr static int align = sizeof(float);
    constexpr static int size = sizeof(float);
};

template <> struct Metric<double> {
    constexpr static int type = enum_cast(TYPE::DOUBLE);
    constexpr static int align = sizeof(double);
    constexpr static int size = sizeof(double);
};

// static auto long_double_type = make_primitive_type<long double>();
//
// static auto signed_char_type = make_primitive_type<signed char>();
// static auto unsigned_char_type = make_primitive_type<unsigned char>();
//
// static auto unsigned_int_type = make_primitive_type<unsigned int>();
// static auto unsigned_short_type = make_primitive_type<unsigned short>();
// static auto unsigned_long_type = make_primitive_type<unsigned long>();
template <> struct Metric<long double> {
    constexpr static int type = enum_cast(TYPE::DOUBLE) | enum_cast(TYPE::LONG);
    constexpr static int align = sizeof(long double);
    constexpr static int size = sizeof(long double);
};

template <> struct Metric<signed char> {
    constexpr static int type = enum_cast(TYPE::SIGNED) | enum_cast(TYPE::CHAR);
    constexpr static int align = sizeof(signed char);
    constexpr static int size = sizeof(signed char);
};

template <> struct Metric<unsigned char> {
    constexpr static int type =
        enum_cast(TYPE::UNSIGNED) | enum_cast(TYPE::CHAR);
    constexpr static int align = sizeof(unsigned char);
    constexpr static int size = sizeof(unsigned char);
};

template <> struct Metric<unsigned int> {
    constexpr static int type =
        enum_cast(TYPE::UNSIGNED) | enum_cast(TYPE::INT);
    constexpr static int align = sizeof(unsigned int);
    constexpr static int size = sizeof(unsigned int);
};

template <> struct Metric<unsigned short> {
    constexpr static int type =
        enum_cast(TYPE::UNSIGNED) | enum_cast(TYPE::SHORT);
    constexpr static int align = sizeof(unsigned short);
    constexpr static int size = sizeof(unsigned short);
};

template <> struct Metric<unsigned long> {
    constexpr static int type =
        enum_cast(TYPE::UNSIGNED) | enum_cast(TYPE::LONG);
    constexpr static int align = sizeof(unsigned long);
    constexpr static int size = sizeof(unsigned long);
};

template <typename T> constexpr static int metric_type_v = Metric<T>::type;
template <typename T> constexpr static int metric_align_v = Metric<T>::align;
template <typename T> constexpr static int metric_size_v = Metric<T>::size;

// 提供从枚举常量直接获取对应类型
// 只有基本类型可以使用
// 这样设计 我们就可以直接从 "int" -> INT -> int !
// 更进一步优化 让他只接收 类型枚举
template <int t> class type_traits;
// 这里的参数可以换成对应的宏 看起来更加清晰
template <> class type_traits<enum_cast(TYPE::VOID)> {
    using type = void;
};
template <> class type_traits<enum_cast(TYPE::CHAR)> {
    using type = char;
};
template <> class type_traits<enum_cast(TYPE::SHORT)> {
    using type = short;
};
template <> class type_traits<enum_cast(TYPE::INT)> {
    using type = int;
};
template <> class type_traits<enum_cast(TYPE::LONG)> {
    using type = long;
};
template <> class type_traits<enum_cast(TYPE::FLOAT)> {
    using type = float;
};
template <> class type_traits<enum_cast(TYPE::DOUBLE)> {
    using type = double;
};

template <int t> using type_traits_t = typename type_traits<t>::type;

// 其实这四个常量是相互一一对应的 从其中一个可以求出另外一个
// 但是没有必要这么麻烦 以后修改的几率很小 就算修改 改起来也很简单
// 所以就这么设计把
constexpr int type_lsh = 0;
constexpr int qualifier_lsh = 8;
constexpr int operator_lsh = 16;
constexpr int operator_qualifier_lsh = 24;
// 3/25 新增
constexpr int storage_class_qualifier_lsh = 24;

constexpr uint32_t type_mask = 0x000000ff;
constexpr uint32_t qualifier_mask = 0x0000ff00;
constexpr uint32_t operator_mask = 0x00ff0000;
constexpr uint32_t operator_qualifier_mask = 0xff000000;
// 3/25 新增
constexpr uint32_t storage_class_qualifier_mask = 0xff000000;

// 如果掩码是32位的那就更加方便了

template <typename Enum> struct enum_traits;

template <> struct enum_traits<TYPE> {
    constexpr static int lsh = type_lsh;
    constexpr static uint32_t mask = type_mask;
};
template <> struct enum_traits<QUALIFIER> {
    constexpr static int lsh = qualifier_lsh;
    constexpr static uint32_t mask = qualifier_mask;
};
template <> struct enum_traits<TYPE_OPERATOR> {
    constexpr static int lsh = operator_lsh;
    constexpr static uint32_t mask = operator_mask;
};
template <> struct enum_traits<OPERATOR_QUALIFIER> {
    constexpr static int lsh = operator_qualifier_lsh;
    constexpr static uint32_t mask = operator_qualifier_mask;
};
template <> struct enum_traits<STORAGE_CLASS_QUALIFIER> {
    constexpr static int lsh = storage_class_qualifier_lsh;
    constexpr static uint32_t mask = storage_class_qualifier_mask;
};
template <typename Enum> constexpr static int lsh = enum_traits<Enum>::lsh;
template <typename Enum>
constexpr static uint32_t mask = enum_traits<Enum>::mask;

template <typename Enum> constexpr uint32_t make_mask(Enum e) noexcept {
    return static_cast<uint32_t>(enum_cast(e)) << lsh<Enum>;
}

// [7,0]
constexpr uint32_t void_mask = make_mask(TYPE::VOID);
constexpr uint32_t char_mask = make_mask(TYPE::CHAR);
constexpr uint32_t short_mask = make_mask(TYPE::SHORT);
constexpr uint32_t int_mask = make_mask(TYPE::INT);
constexpr uint32_t long_mask = make_mask(TYPE::LONG);
constexpr uint32_t float_mask = make_mask(TYPE::FLOAT);
constexpr uint32_t double_mask = make_mask(TYPE::DOUBLE);
constexpr uint32_t variable_mask = make_mask(TYPE::VARIABLE);

// [15,8]
constexpr uint32_t const_mask = make_mask(QUALIFIER::CONST);
constexpr uint32_t volatile_mask = make_mask(QUALIFIER::VOLATILE);
constexpr uint32_t unsigned_mask = make_mask(QUALIFIER::UNSIGNED);
constexpr uint32_t signed_mask = make_mask(QUALIFIER::SIGNED);

// [23,16]
constexpr uint32_t array_mask = make_mask(TYPE_OPERATOR::ARRAY);
constexpr uint32_t pointer_mask = make_mask(TYPE_OPERATOR::POINTER);
constexpr uint32_t struct_mask = make_mask(TYPE_OPERATOR::STRUCT);
constexpr uint32_t union_mask = make_mask(TYPE_OPERATOR::UNION);
constexpr uint32_t function_mask = make_mask(TYPE_OPERATOR::FUNCTION);
constexpr uint32_t enum_mask = make_mask(TYPE_OPERATOR::ENUM);

// [31,24]
constexpr uint32_t const_operator_mask = make_mask(OPERATOR_QUALIFIER::CONST);
constexpr uint32_t volatile_operator_mask =
    make_mask(OPERATOR_QUALIFIER::VOLATILE);

// 不对不对 链接属性不属于类型
// 所以这块设计就没有必要了
// 链接属性是一个单独的参数 在声明符号时才有用
// 所以在Symbol里可以有一个域表示链接属性 storage class
// 类型系统就没有必要了
// 3/25
constexpr uint32_t auto_storage_mask = make_mask(STORAGE_CLASS_QUALIFIER::AUTO);
constexpr uint32_t extern_storage_mask =
    make_mask(STORAGE_CLASS_QUALIFIER::EXTERN);
constexpr uint32_t typedef_storage_mask =
    make_mask(STORAGE_CLASS_QUALIFIER::TYPEDEF);
constexpr uint32_t register_storage_mask =
    make_mask(STORAGE_CLASS_QUALIFIER::REGISTER);
constexpr uint32_t static_storage_mask =
    make_mask(STORAGE_CLASS_QUALIFIER::STATIC);
_CML_END_
#endif // _ZCC_TYPE_METRIC_H_