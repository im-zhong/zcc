
/*
        原来的type.h文件中的Type移动到native type.h
        zhong, ysu
        3.3 21:11



        TODO type类型的虚函数 重新设计
        to_string
        is_const
        is_volatile
        is_native
        剩下的全部变成工厂函数

        TODO 针对所有的每一个类型提供一个头文件
        pointer array function
        struct union enum

        TODO
                实现一个align函数 这样Type就只需要两个参数了 type_word 和 size
*/
#ifndef _ZCC_PRIMITIVE_TYPE_H_
#define _ZCC_PRIMITIVE_TYPE_H_

#include "type_assert.h"
#include "type_metric.h"
#include <memory>


_CML_BEGIN_

// 因为Type的类成员是使用的如此频繁 所以作为公开成员 不用提供函数了
// 然后除了必要的类成员函数 均使用全局函数 使用std::shared_ptr<Type> 作为参数
class Type {
  public:
    Type(uint32_t type_word, int size)
        : type_word(type_word), size(size), align(size) {}
    Type(uint32_t type_word, int size, int align)
        : type_word(type_word), size(size), align(align) {}
    Type() = default;
    Type(const Type&) = default;
    Type& operator=(const Type&) = default;
    Type(Type&&) = default;
    Type& operator=(Type&&) = default;
    ~Type() = default;

    // TODO 3.3
    // 这些成员函数太复杂 我尝试实现一下类型检查
    // 看看那些函数合适作为成员 那些函数合适作为全局

    // int get_align() const noexcept { return align; }
    // int get_size() const noexcept { return size; }

    // void set_align(int align) noexcept { this->align = align; }
    // void set_size(int size) noexcept { this->size = size; }

    // 这些在全局实现 然后这里调用即可
    int get_operator() const noexcept { return type_word >> operator_lsh; }
    int get_type() const noexcept { return type_mask & type_word; }

    // void clear_operator() noexcept { type_word &= 0x0000FFFFu; }
    // void clear_qualifier() noexcept { type_word &= 0xFFFF00FFu; }
    // void clear_type() noexcept { type_word &= 0xFFFFFF00u; }

    // 这里也改成operator_mask

    // set系函数全部改成make_pointer
    // void set_operator(TYPE_OPERATOR type_operator) noexcept
    //{
    //	uint32_t op = enum_cast(type_operator);
    //	op <<= 16; // 低位必须填0 否则会出错
    //	clear_operator();
    //	type_word |= op;
    //}

    // void set_type(TYPE native_type) noexcept
    //{
    //	uint32_t type = enum_cast(native_type);
    //	clear_type();
    //	type_word |= type;
    // }

    bool is_completed() const noexcept { return size != 0; }

    // qualifier [s,0,0,0,0,0,c,v]
    // s = 0 : signed; s = 1 : unsigned
    // c = 0 : non const; c = 1 : const
    // v = 0 : non volatile; v = 1 : volatile
    // constexpr static uint32_t const_mask = 0x00000200u;
    // constexpr static uint32_t volatile_mask = 0x00000100u;
    // virtual bool is_const() const noexcept
    //{
    //	// 获得const对应的位置
    //	// 我应该把这些特殊的常量用掩码的方式写出来

    //	return const_mask & type_word;
    //}

    // virtual bool is_volatile() const noexcept
    //{

    //	return volatile_mask & type_word;
    //}

    // bool is_const_volatile() const noexcept
    //{
    //	return is_const() && is_volatile();
    // }

    // constexpr static uint32_t signed_mask = 0u;
    // constexpr static uint32_t unsigned_mask = 0x00008000u;

    // bool is_unsigned() const noexcept
    //{
    //	return unsigned_mask & type_word;
    // }

    // bool is_signed() const noexcept
    //{
    //	return !is_unsigned();
    // }

    // 其他派生类型继承返回假就行了 我真是机智
    // virtual bool is_native() const noexcept
    //{
    //	return true;
    //}

    // 修改为make系函数
    // void set_const() noexcept
    //{
    //	type_word |= const_mask;
    //}

    // void set_volatile() noexcept
    //{
    //	type_word |= volatile_mask;
    // }

    // void set_const_volatile() noexcept
    //{
    //	set_const();
    //	set_volatile();
    // }

    // virtual std::shared_ptr<Type> make_const()
    //{
    //	auto const_type = std::make_shared<Type>(*this); // 复制构造函数
    //	set_qualifier(const_type->type_word, enum_cast(QUALIFIER::CONST));
    //	return const_type;
    // }

    // virtual std::shared_ptr<Type> make_volatile()
    //{
    //	auto volatile_type = std::make_shared<Type>(*this);
    //	set_qualifier(volatile_type->type_word, enum_cast(QUALIFIER::VOLATILE));
    //	return volatile_type;
    // }

    // void remove_const() noexcept
    //{
    //	// 对应位设置0
    //	type_word &= ~const_mask;
    // }

    // void remove_volatile() noexcept
    //{
    //	type_word &= ~volatile_mask;
    // }

    // void remove_const_volatile() noexcept
    //{
    //	remove_const();
    //	remove_volatile();
    // }

    // void set_signed() noexcept
    //{
    //	// unsigned_mask 对应的位置0
    //	type_word &= ~unsigned_mask;
    // }
    // void set_unsigned() noexcept
    //{
    //	type_word |= unsigned_mask;
    // }

    // virtual std::shared_ptr<Type> get_inner_type() const { return nullptr; }

    virtual std::string to_string() const noexcept;

    uint32_t get_type_word() const noexcept { return type_word; }
    int get_size() const noexcept { return size; }
    int get_align() const noexcept { return align; }

  public:
    // 给这个玩意改个名字 叫做 type_wrod [op|qualifier|type]
    // high 16 bit = operator
    // low 16 bit - high 8 bit = qulifier
    //				low 8 bit  = native type
    std::uint32_t type_word{};
    int size{};  // 类型大小
    int align{}; // 类型在本机器上的对齐大小
};

// 首先创建原生类型
template <typename T> std::shared_ptr<Type> make_primitive_type() {
    // 我应该能够根据T获得相应的metric
    return std::make_shared<Type>(Metric<T>::type, Metric<T>::align,
                                  Metric<T>::size);
}

// 只有这些类型是不够的  我要为所有的基础类型的所有组合生成一个单位
// 这样会让类型系统简单非常多
static auto void_type = make_primitive_type<void>();
static auto char_type = make_primitive_type<char>();
static auto float_type = make_primitive_type<float>();
static auto double_type = make_primitive_type<double>();
static auto int_type = make_primitive_type<int>();

static auto short_type = make_primitive_type<short>();
static auto long_type = make_primitive_type<long>();
static auto long_double_type = make_primitive_type<long double>();

static auto signed_char_type = make_primitive_type<signed char>();
static auto unsigned_char_type = make_primitive_type<unsigned char>();

static auto unsigned_int_type = make_primitive_type<unsigned int>();
static auto unsigned_short_type = make_primitive_type<unsigned short>();
static auto unsigned_long_type = make_primitive_type<unsigned long>();

static auto variable_type = std::make_shared<Type>(variable_mask, 0);
_CML_END_
#endif // _ZCC_PRIMITIVE_TYPE_H_