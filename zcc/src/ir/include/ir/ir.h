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
        ADDROF,
        LOAD,
        STORE,
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

        // cast
        BITCAST,
        ZEROEXT,
        SIGNEXT,
        TRUNC,
        FEXT,
        FTRUNC,
        // pointer类型支持数学运算
    };
};

std::string type_to_string(Type type);
std::string op_to_string(int op);

struct Symbol {
    std::string name;
    Type type;
    std::string to_string() const;
};
using SymbolList = std::vector<Symbol>;
auto make_empty_symbol_list() -> SymbolList;

// 不行 对于一些常用的指针类型 可以使用using std::shared_ptr<Symbol>
// three address code
// 来自龙书 indirect triple
struct Instruction {
    int op;
    Symbol left;
    std::optional<Symbol> right;
    Symbol result;
    std::string to_string() const;
};
using InstructionPtr = std::shared_ptr<Instruction>;
auto make_instruction(Instruction instruction) -> InstructionPtr;

// 是在源代码中的位置
struct Label {
    std::string name;
    std::string to_string() const;
};
using LabelPtr = std::shared_ptr<Label>;
LabelPtr make_label(Label label);

struct Branch {
    std::optional<Symbol> condition;
    std::string true_label;
    std::optional<std::string> false_label;
    std::string to_string() const;
};
using BranchPtr = std::shared_ptr<Branch>;
BranchPtr make_branch(Branch branch);

struct FnCall {
    // 调用的函数名字
    std::string name;
    // 返回符号 但是可选
    // std::optional<std::string> result;
    // // 返回类型 可选
    // std::optional<Type> return_type;
    // 参数列表在函数声明和函数定义中是一样的 可以定义一个typedef
    // 因为后面会修改type的实现那
    SymbolList parameter_list;
    std::optional<Type> return_type;
    std::optional<Symbol> result;
    std::string to_string() const;
};
using FnCallPtr = std::shared_ptr<FnCall>;
FnCallPtr make_fncall(FnCall fncall);

struct Return {
    std::optional<Symbol> return_value;
    std::string to_string() const;
};
using ReturnPtr = std::shared_ptr<Return>;
ReturnPtr make_return(Return ret);

struct SymbolDef {
    bool local;
    Type type;
    Symbol result;
    std::optional<Symbol> size;
    std::string to_string() const;
};
using SymbolDefPtr = std::shared_ptr<SymbolDef>;
SymbolDefPtr make_symbol_def(SymbolDef symbol_def);

struct Cast {
    int cast;
    Symbol value;
    Type type;
    Symbol result;
    std::string to_string() const;
};
using CastPtr = std::shared_ptr<Cast>;
CastPtr make_cast(Cast cast);

struct Gep {
    Symbol value;
    Symbol array_index;
    Symbol struct_index;
    Symbol result;
    std::string to_string() const;
};
using GepPtr = std::shared_ptr<Gep>;
GepPtr make_gep(Gep gep);

using Code = std::variant<InstructionPtr, LabelPtr, BranchPtr, FnCallPtr,
                          ReturnPtr, SymbolDefPtr, CastPtr, GepPtr>;
using CodeList = std::vector<Code>;
CodeList make_empty_code_list();

// 这个实际上是定义 而不是声明
struct FnDef {
    // 函数名字
    std::string name;
    // 函数返回类型
    // 参数列表
    SymbolList parameter_list;
    CodeList body;
    std::optional<Type> return_type;
    // 函数
    std::string to_string() const;
};
using FnDefPtr = std::shared_ptr<FnDef>;
FnDefPtr make_fndef(FnDef fndef);

// todo
struct SymbolDecl {
    Symbol symbol;
    std::string to_string() const;
};
using SymbolDeclPtr = std::shared_ptr<SymbolDecl>;
SymbolDeclPtr make_symbol_decl(SymbolDecl symbol_decl);

// 用来统一global 和 local

struct StructDecl {
    // type list
    std::string name;
    TypeList fields;
    std::string to_string() const;
};
using StructDeclPtr = std::shared_ptr<StructDecl>;
StructDeclPtr make_struct_decl(StructDecl struct_decl);

using Decl = std::variant<FnDefPtr, StructDeclPtr, SymbolDefPtr, SymbolDeclPtr>;
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
