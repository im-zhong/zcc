// 2023/2/25
// zhangzhong
// test ir

// 一趟解析结束之后应该生成一份 符号表 类型表 指令表 标号表

// doctest 还必须加上这个宏 不然通不过编译
// #define DOCTEST_CONFIG_IMPLEMENT

#include "ir/driver.h"
#include "ir/ir.h"
#include "ir/parser.hpp"
#include "ir/type.h"
// #include "util/doctest.h" // 这东西不仅没提供conan2 还有bug。。。 算了
// 还是换googletest吧
#include <cassert>
#include <gtest/gtest.h>
#include <optional>
#include <string>

// 可以在这里写测试函数呀
void test_type() {
    using ir::IR;
    using namespace std::string_literals;

    // assert无法满足测试的需求 换用另外一个测试框架
    // doctest
    auto pi = ir::make_pointer_type(make_basic_type(IR::I32));
    EXPECT_EQ(ir::to_string(*pi), "*i32"s);
    EXPECT_EQ(ir::type_to_string(pi), "*i32"s);

    // test function type
    // fn(bool, *i32)->i32
    auto type_list = ir::make_empty_type_list();
    type_list.push_back(ir::make_basic_type(IR::BOOL));
    type_list.push_back(pi);
    auto fn = ir::make_fn_type({.parameter_type = type_list,
                                .return_type = ir::make_basic_type(IR::I32)});
    EXPECT_EQ(ir::to_string(*fn), "fn(bool, *i32)->i32"s);
    EXPECT_EQ(ir::type_to_string(fn), "fn(bool, *i32)->i32"s);

    // test return
    auto ret = ir::make_return({});
    EXPECT_EQ(ir::to_string(*ret), "ret none");
    auto ret2 = ir::make_return(
        {.return_value =
             ir::Symbol{.name = "%1", .type = ir::make_basic_type(IR::I32)}});
    EXPECT_EQ(ir::to_string(*ret2), "ret %1:i32");
}

void test() { test_type(); }

int main(int argc, char* argv[]) {
    test();

    auto drv = ir::driver{};
    // 然后读取用户输入的文件
    for (int i = 1; i < argc; i++) {
        drv.parse(argv[i]);
    }

    // 然后输出
    drv.dump();
    return 0;
}
