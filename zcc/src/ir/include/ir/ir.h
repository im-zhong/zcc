// 2023/2/24
// zhangzhong
// ir, Intermediate Representation

#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// 还需要一个symbol table

namespace ir {

// todo
// 这个东西需要保存类型
struct Type {};
Type make_basic_type(int basic_type);
Type make_struct_type();
Type make_pointer_type();

// 定义一些常量, terminal
struct IR {
    enum Code {
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

        BITNOT,

        // operator
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

class _Symbol;
using Symbol = std::shared_ptr<_Symbol>;

// 不行 对于一些常用的指针类型 可以使用using std::shared_ptr<Symbol>
// three address code
// 来自龙书 indirect triple
struct _Instruction {
    int op;
    // 这个名字太长了，可以typedef using 一下
    // Address left;
    // Address right;
};

struct BinaryAssignemt {};

// 是在源代码中的位置
struct Label {};

using Instruction = std::shared_ptr<_Instruction>;

struct _Instruction;
using Address = std::variant<Symbol, Instruction>;

// 在构造指令的时候 需要四个参数
// 但是在保存的时候 却只需要三个参数
Instruction make_instruction();

Instruction make_binary_assignment(int op, std::string left, int lty,
                                   std::string right, int rty,
                                   std::string result);

std::shared_ptr<Instruction> make_unary_assignment(int op, Address num);

// 为了让parser可以构造triple 我们需要提供一些函数
// 给各种指令提供

// 我们读入ir 最终生成的就是一个 ir指令列表
// 同时还有一个符号表
// 同时还有一些函数
// 总之就是提供一些cgen需要用到的环境，然后cgen来根据这些结构生成代码

class IRParser {
  public:
    // 和 bison 一样
    // 我们只需要指定一个文件 或者指定一个输入流
    // 就可以从中读取文件
    // 但是讲道理 我们会使用bison来做的话 我们其实只需要向instructions中添加
    // 指令即可

  private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbol_table;
    std::unordered_map<std::string, std::shared_ptr<Instruction>> labels;
    std::vector<std::shared_ptr<Instruction>> instructions;
};

} // namespace ir
