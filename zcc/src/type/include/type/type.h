
/*
        zhong, ysu
        3.3
        TOOD type模块也已经基本完成 除了Enumerate不知道怎么处理之外
                但是这个文件太大了 最好可以根据类型将头文件分开
                其中的函数实现都搬到cpp文件里 不然会报链接错误

                如何分开
                 1. 将Type 类型 重新写道 native_type.h 头文件中

        3.25
        再次重构了类型系统 现在的类型系统设计非常简单 没有考虑错误情况
        实际上我们并不需要考虑错误情况 因为错误的情况会在语义检查阶段报错
        我们不需要为错误的声明生成错误的类型 所以类型系统的设计没有必要考虑太多
        之前是我考虑太多才导致类型系统一直重构 一直写不好
*/

#ifndef _ZCC_TYPE_H_
#define _ZCC_TYPE_H_

// 类型分为 基础类型  复杂类型
// 并且类型还有修饰词 qualifier const volatile pointer unsigned signed
// 修饰词可以只使用一个变量的不同位类存储

/*
        3.2
        思考 我究竟在什么使用会用到 Type
        我从词法分析可可以得到什么
        我从词法分析器只能得到一个 "int" 而已 他是一个字符串
        所以如果我希望从 "int" 转换到 int_type 只需要实现一个hash_map即可
        但是如果是int 那么我为什么要创建一个类型呢 并不需要
        有时候只是 我们知道了我们现在想要创建一个 int 类型的值
        所以我们应该提供这样的函数  make<int>(value)  来创建一个类型

        假设前端读到了 "int" 因为是运行期 我们仅有可能根据一些判断 返回一个代表
   "int" 的值 假设这个值是常量 INT 那么我们需要根据INT这个常量来定义一个
   在我们的编译器中代表int类型的变量 Type int_type 这样写因为int_type  char_type
   实际上都是 Type 所以必然不可能在从其中萃取出原来的类型 int char

        如果能萃取出来是非常方便的 因为 如果我们创建了 int_tpye 同时也想创建一个
   int 类型的值 上面这种设计是做不到的 我们只能根据 INT的值再次调用一次 switch
   而且因为返回值不同 我们必须为每一个值提供一个函数 有几种类型就写几个 非常麻烦

*/
// #include "symbol.h"

// class Symbol{};
// 前置声明
// 符号类型

// 用八个位来表示一个类型
// 类型 = 修饰符 + 基本类型
// 低四位表示类型
// 高四位表示 const volatile signed unsigned

// 基本类型用一个类来表示
// 再用八个位表示 作用在类型上的操作 int 0000 0010
//  const int 0001 0010
// unsigned cosnt volatile int 0111 0010
// 这样八位就可以表示几乎所有的 原生类型的所有修饰

// 如果我想表示一个指针呢 int *
// op = POINTER , type = 0000 0010

// 如果我想表示一个指针数组呢 (int*)[10]
// op = ARRAY , type = (op = POINTER, type = 0000 0010) 还有一个10 需要表示 size
// = 10 * size of (inner type)

// 如果我想表示一个指针数组的指针 那么就是
// op = POINTER, type = (... 上面的东西) size = sizeof(POINTER)

// 如果我想表示一个结构体
// op = STRUCT, name = struct_name , Filed
// 结构体还需要保存域内的类型和变量
// Field = {Type name; Type name}

// 类型实际上是一个链
// 如果你想描述 一个 指向数组的指针 并且数组元素是指向函数的指针
// pointer to array [pointer to function] 这不是一个类型可以完成的
// 并且简单类型没有Field
// 复杂类型存在域

// 不对 原生类型的类别是一个 uint8

#include "array_type.h"
#include "enum_type.h"
#include "function_type.h"
#include "pointer_type.h"
#include "qualifier_type.h"
#include "struct_type.h"
#include "union_type.h"

// std::shared_ptr<Type> make_const(std::shared_ptr<Type> type);
// std::shared_ptr<Type> make_volatile(std::shared_ptr<Type>& type);
// std::shared_ptr<Type> make_const_volatile(std::shared_ptr<Type>& type);
// std::shared_ptr<Type> make_qualifier(std::shared_ptr<Type>& type);
// std::shared_ptr<Type> remove_qualifier(std::shared_ptr<Type>& type);

// 创建类型并保存到符号表
// Type 类型没有必要创建很多个 相同的类型保存一份就可以了
// 这样比较两个类型是否相等只需要比较指针是否相同
// void make_type();

// 从操作数转换到相应的Type类型
// template <TYPE_OPERATOR op>
// class type_operator_cast;
//
// template <>
// struct type_operator_cast<TYPE_OPERATOR::ARRAY>
//{
//	using type = ArrayType;
//};

#endif // !_ZCC_TYPE_H_
