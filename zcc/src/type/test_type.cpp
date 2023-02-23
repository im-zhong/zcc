/*
        type

        TODO 查看智能指针 关于 dynamic 指针转换的部分
                 完成Symbol的基础功能 先不实现符号表
                 将Symbol和Type结合起来 尝试构建所有类型 并检测正确性
                 DONE

        TOOD 3.25的任务
        3/24 重构Type 放弃通过地址来比较类型相同的想法 这个想法限制太大
   难以灵活实现 仍然沿用type_word思想 但是需要在类型中加上storage class字段
        完成之后测试类型表和符号表 完成声明语义分析的前三个部分
   就是我今天写的那几个部分 包括基础类型 指针 数组 不包括函数 结构体 枚举

        然后实现表达式语义分析和语义计算
        然后实现语句
        最后再实现声明剩下的部分

        然后设计中间代码形式 生成中间代码
        能写到这里真的很不错了 不要给自己太大压力 能生成中间代码就可以了
*/

#include "type/type.h"
#include <iostream>
// #include "Symbol/symbol_test.h"
#include "symbol/symbol.h"
#include "type/type_table.h"
using namespace std;
using namespace cml;

void print_array_type(std::shared_ptr<Type> type) {
    auto op = type->get_operator();

    auto array_type = std::dynamic_pointer_cast<ArrayType>(type);

    // 最好我可以直接从 op 获得 对应的继承类的类型 使用元模板计数
    // 然后再用 dynamic_cast  转换到对应的类型 就可以直接调用其静态函数
    cout << array_type->size << endl;

    if (array_type->get_element_type()) {
        cout << "inner type size ";
        cout << array_type->get_element_type()->size << endl;
    }
}

// 简单的写一些类型比较
// bool is_equal(std::shared_ptr<Type> lhs, std::shared_ptr<Type> rhs)
//{
//	if (lhs && rhs)
//	{
//		auto op_lhs = lhs->get_operator();
//		auto op_rhs = rhs->get_operator();
//
//		if (op_lhs == op_rhs) // 第一步相等了 才有比较的可能
//		{
//			// 然后递归比较inner_type
//			return is_equal(lhs->get_inner_type(),
// rhs->get_inner_type());
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return false;
//
//}

void Test_type_1() {
    auto int_array_10 = make_array(int_type, 10);
    auto taowa = make_array(int_array_10, 5);

    cout << int_array_10->to_string() << endl;
    cout << taowa->to_string() << endl;

    ////// 如果能输出类型就好了
    print_array_type(int_array_10);
    print_array_type(taowa);

    auto void_pointer = make_pointer(void_type);
    cout << void_pointer->to_string() << endl;

    for (int i = 0; i < 100; ++i) {
        cout << make_random_string(31) << endl;
    }

    //// 创建一个符号
    auto symbol = make_symbol();
    auto str = symbol->to_string();
    cout << str << endl;

    auto int_symbol = make_symbol("a", 0, int_type);
    auto int_str = int_symbol->to_string();
    cout << int_str << endl;
    //

    //// 好的 我现在要声明一个结构
    //// struct Point
    ////  int x;
    ////  int y;
    //// 那么首先我应该创建一个Field
    //// 用Symbol创建Field
    //// 用Field创建创建StructField
    //// 用StructField 创建Struct

    //// 那么我们怎么创建一个域呢
    auto x_symbol = make_symbol("x", enum_cast(SCOPE::LOCAL), int_type);
    auto y_symbol = make_symbol("y", enum_cast(SCOPE::LOCAL), int_type);

    //// 然后创建两个域
    ////auto struct_field = make_struct_field();
    //////struct_field->push(x_symbol);
    //////struct_field->push(y_symbol);

    ////// 然后从两个域创建一个StructField

    ////// TODO
    ////// 这样做是不对的 我们可以直接从一个
    /// forward_list<std::shared_ptr<Symbol>> 中自动生成一个域
    ////// 或者我们可以生成一个StructField 然后不断向其中添加域 自动更新offset
    /// 就好了

    ////
    auto simple_struct_type = make_struct("Point", true);
    simple_struct_type->insert(x_symbol);
    simple_struct_type->insert(y_symbol);
    cout << simple_struct_type->to_string() << endl;

    auto simple_union_type = make_union("Point", true);
    simple_union_type->insert(x_symbol);
    simple_union_type->insert(y_symbol);
    cout << simple_union_type->to_string() << endl;

    auto cs = make_const(simple_struct_type);
    auto cvs = make_const_volatile(simple_struct_type);
    cout << cs->to_string() << endl;
    cout << cvs->to_string() << endl;

    //// 创建函数类型
    auto function_type = make_function(
        int_type, std::list<std::shared_ptr<Type>>{int_type, variable_type});
    cout << function_type->to_string() << endl;

    // symbol test
    // symbol_test();
}

void type_talbe_test() { global_type_table.insert("int", int_type); }

int main() {
    // 3.6
    Test_type_1();
    // 3.25
    type_talbe_test();
}