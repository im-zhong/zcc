// #pragma once
///*
//	constant
//*/
//
// #include <zcc/table/table.h>
//
//// 这就是一个常量 他又类型和值
// template <typename T>
// struct Constant
//{
//	Type type;
//	T value;
// };
//
//// 类似这样的
// struct CharConstant
//{
//	Type type; // chartype
//	char value;
// };
//
//// 因为只有有限的值可以表示成常量
//// 所以针对每一个常量表提供一个特例化即可 这样还更加高效
//
// template <typename T>
// class ConstantTable : public Table
//{
//
//	// 在编译器出现的常量，常处理成指向常量符号的指针
//	// 这些指针保存在constant表中
//	// constant表与作用域无关 scope = CONSTANT
//
//	// 我需要能够保存所有常量
//
//	// 对每个常量来说 它应该仅在常量中保存一份实例
//
//	// 默认实现 常量表里的值不会删除
//
// public:
//
//
//	std::shared_ptr<Constant<T>> make_constant(T value)
//	{
//		// 我应该能够从T推导☞ inttype chartype 之类的
//
//		// 在这里应该比较两个常量是否相同 因为常量已经按照类型区分
//		// 所以这里只需要比较值是否相同即可
//
//	}
//
//
//
//};
//
//
//// 不应该让用户每次调用常量都 intconsttable.make ... charconsttable.make ,...
//// 使用元编程中的 标签分派 来自动分派