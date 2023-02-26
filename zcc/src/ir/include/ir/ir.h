// 2023/2/24
// zhangzhong
// ir, Intermediate Representation

#pragma once

// 我服了 23年了 c++20 还没支持完呢。。。
// #include <format>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// 还需要一个symbol table

// 放在外面简单一点 大家都可以用
template <typename T>
concept to_string_able = requires(T&& t) {
                             // 这个类型必须定义了to_string
                             std::forward<T>(t).to_string();
                             // 并且这个函数必须返回 std::string
                             {
                                 std::forward<T>(t).to_string()
                                 } -> std::same_as<std::string>;
                         };

namespace ir {

template <to_string_able T> auto to_string(T&& t) -> std::string {
    return std::forward<T>(t).to_string();
}

// todo
// 这个东西需要保存类型
struct Type {};
Type make_basic_type(int basic_type);
Type make_struct_type();
Type make_pointer_type();

// 定义一些常量, terminal
struct IR {
    enum Coding {
        // keyword
        GLOBAL = 256,
        LOCAL,
        IF,
        THEN,
        ELSE,
        DECL,
        FN,
        STRUCT,
        TYPE,

        // basic type
        BOOL,
        I8,
        I16,
        I32,
        I64,
        F32,
        F64,

        // operator
        ASSIGN,
        ADD,
        SUB,
        MUL,
        DIV,
        BITAND,
        BITOR,
        BITXOR,
        LSH,
        RSH,
        MOD,
        LESS,
        LESS_QEUAL,
        GREAT,
        GREAT_EQUAL,
        EQUAL,
        NOT_EQUAL,

        // symbol
        GLOBAL_SYMBOL,
        LOCAL_SYMBOL,
        TEMP_SYMBOL,
        NUM,
        ERROR,
    };
};

std::string type_to_string(int type);
std::string op_to_string(int op);

// struct Code {
//     virtual std::string to_string() const = 0;
// };
// using CodePtr = std::shared_ptr<Code>;

// 不行 对于一些常用的指针类型 可以使用using std::shared_ptr<Symbol>
// three address code
// 来自龙书 indirect triple
struct Instruction {
    int op;
    std::string left;
    int left_type;
    std::string right;
    int right_type;
    std::string result;

    std::string to_string() const;
};
using InstructionPtr = std::shared_ptr<Instruction>;
InstructionPtr make_binary_assignment(int op, std::string left, int lty,
                                      std::string right, int rty,
                                      std::string result);

InstructionPtr make_assignment(std::string left, int left_type,
                               std::string result);

// 是在源代码中的位置
struct Label {
    std::string label;
    std::string to_string() const;
};
using LabelPtr = std::shared_ptr<Label>;
LabelPtr make_label(std::string label);

using Code = std::variant<InstructionPtr, LabelPtr>;
using CodeList = std::vector<Code>;
CodeList make_empty_code_list();

// 为了让parser可以构造triple 我们需要提供一些函数
// 给各种指令提供

// 我们读入ir 最终生成的就是一个 ir指令列表
// 同时还有一个符号表
// 同时还有一些函数
// 总之就是提供一些cgen需要用到的环境，然后cgen来根据这些结构生成代码

} // namespace ir
