// 2023/2/24
// zhangzhong
// ir, Intermediate Representation

#pragma once

// 我服了 23年了 c++20 还没支持完呢。。。
// #include <format>
#include "type.h"
#include <list>
#include <memory>
#include <optional>
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

std::string type_to_string(Type type);
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
    Type left_type;
    std::string right;
    Type right_type;
    std::string result;

    std::string to_string() const;
};
using InstructionPtr = std::shared_ptr<Instruction>;
InstructionPtr make_binary_assignment(int op, std::string left, Type lty,
                                      std::string right, Type rty,
                                      std::string result);

InstructionPtr make_assignment(std::string left, Type left_type,
                               std::string result);

// 是在源代码中的位置
struct Label {
    std::string label;
    std::string to_string() const;
};
using LabelPtr = std::shared_ptr<Label>;
LabelPtr make_label(std::string label);

struct TypedSymbol {
    std::string symbol;
    Type type;
};

using SymbolList = std::vector<TypedSymbol>;
auto make_empty_symbol_list() -> SymbolList;

struct Branch {
    std::optional<TypedSymbol> condition;
    std::string true_label;
    std::optional<std::string> false_label;

    std::string to_string() const;
};
using BranchPtr = std::shared_ptr<Branch>;
BranchPtr make_full_branch(std::string symbol, Type type,
                           std::string true_label, std::string false_label);
BranchPtr make_half_branch(std::string symbol, Type type,
                           std::string true_label);
BranchPtr make_goto(std::string label);

struct FnCall {
    // 调用的函数名字
    std::string name;
    // 返回符号 但是可选
    std::optional<std::string> result;
    // 返回类型 可选
    std::optional<Type> return_type;
    // 参数列表在函数声明和函数定义中是一样的 可以定义一个typedef
    // 因为后面会修改type的实现那
    SymbolList parameter_list;

    std::string to_string() const;
};
using FnCallPtr = std::shared_ptr<FnCall>;
FnCallPtr make_fncall(std::string name, SymbolList parameter_list);
FnCallPtr make_fncall_assignment(std::string name, SymbolList parameter_list,
                                 std::string result, Type return_type);

struct Return {
    std::optional<TypedSymbol> return_value;

    std::string to_string() const;
};
using ReturnPtr = std::shared_ptr<Return>;
ReturnPtr make_return(std::optional<TypedSymbol> return_value = std::nullopt);

struct LocalDecl {
    std::string name;
    Type type;

    std::string to_string() const;
};
using LocalDeclPtr = std::shared_ptr<LocalDecl>;
LocalDeclPtr make_local_decl(std::string name, Type type);

struct AddrOf {
    std::string result;
    std::string value;
    Type type;

    std::string to_string() const;
};
using AddrOfPtr = std::shared_ptr<AddrOf>;
AddrOfPtr make_addrof(std::string result, std::string value, Type type);

struct Load {
    std::string result;
    std::string value;
    Type type;

    std::string to_string() const;
};
using LoadPtr = std::shared_ptr<Load>;
LoadPtr make_load(std::string result, std::string value, Type type);

using Code = std::variant<InstructionPtr, LabelPtr, BranchPtr, FnCallPtr,
                          ReturnPtr, LocalDeclPtr, AddrOfPtr, LoadPtr>;
using CodeList = std::vector<Code>;
CodeList make_empty_code_list();

struct FnDecl {
    // 函数名字
    std::string name;
    // 函数返回类型
    // 参数列表
    SymbolList parameter_list;
    std::optional<Type> return_type;
    // 函数体
    CodeList body;

    std::string to_string() const;
};
using FnDeclPtr = std::shared_ptr<FnDecl>;
FnDeclPtr make_fndecl(std::string name, SymbolList parameter_list,
                      Type return_type, CodeList body);
FnDeclPtr make_noret_fndecl(std::string name, SymbolList parameter_list,
                            CodeList body);

struct StructDecl {
    // type list
    std::string name;
    TypeList fields;

    std::string to_string() const;
};
using StructDeclPtr = std::shared_ptr<StructDecl>;
StructDeclPtr make_struct_decl(std::string name, TypeList fields);

struct GlobalDecl {
    std::string name;
    std::string value;
    Type type;

    std::string to_string() const;
};
using GlobalDeclPtr = std::shared_ptr<GlobalDecl>;
GlobalDeclPtr make_global_decl(std::string name, std::string value, Type type);

using Decl = std::variant<FnDeclPtr, StructDeclPtr, GlobalDeclPtr>;
using DeclList = std::vector<Decl>;
DeclList make_empty_decl_list();

// function就是另外一条赛道了

// 为了让parser可以构造triple 我们需要提供一些函数
// 给各种指令提供

// 我们读入ir 最终生成的就是一个 ir指令列表
// 同时还有一个符号表
// 同时还有一些函数
// 总之就是提供一些cgen需要用到的环境，然后cgen来根据这些结构生成代码

} // namespace ir
