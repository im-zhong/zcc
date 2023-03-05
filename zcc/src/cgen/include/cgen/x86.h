// 2023/3/4
// zhangzhong
// x86 specification

#pragma once

#include "cgen.h"
#include "ir/ir.h"
#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cgen {

// 这个东西本质上来说就是一个map
// 我们只需要简单的将ir里面的东西复制过来就行了
// 2023/2/24
// zhangzhong
// ir, Intermediate Representation

// 我服了 23年了 c++20 还没支持完呢。。。
// #include <format>

// 还需要一个symbol table

// 放在外面简单一点 大家都可以用
// template <typename T>
// concept stringable = requires(T&& t) {
//                          // 这个类型必须定义了to_string
//                          std::forward<T>(t).to_string();
//                          // 并且这个函数必须返回 std::string
//                          {
//                              std::forward<T>(t).to_string()
//                              } -> std::same_as<std::string>;
//                      };

// template <stringable T> auto to_string(T&& t) -> std::string {
//     return std::forward<T>(t).to_string();
// }

// // 定义一些常量, terminal
// struct IR {
//     enum Coding {
//         // keyword
//         GLOBAL = 256,
//         LOCAL,
//         IF,
//         THEN,
//         ELSE,
//         DECL,
//         FN,
//         STRUCT,
//         TYPE,

//         // basic type
//         BOOL,
//         I8,
//         I16,
//         I32,
//         I64,
//         F32,
//         F64,

//         // operator
//         ASSIGN,
//         ADDROF,
//         LOAD,
//         STORE,
//         ADD,
//         SUB,
//         MUL,
//         DIV,
//         BITAND,
//         BITOR,
//         BITXOR,
//         LSH,
//         RSH,
//         MOD,
//         LESS,
//         LESS_QEUAL,
//         GREAT,
//         GREAT_EQUAL,
//         EQUAL,
//         NOT_EQUAL,

//         // symbol
//         GLOBAL_SYMBOL,
//         LOCAL_SYMBOL,
//         TEMP_SYMBOL,
//         NUM,
//         ERROR,

//         // cast
//         BITCAST,
//         ZEROEXT,
//         SIGNEXT,
//         TRUNC,
//         FEXT,
//         FTRUNC,
//         // pointer类型支持数学运算
//     };
// };

// std::string op_to_string(int op);

// struct Symbol {
//     std::string name;
//     int type;
//     std::string to_string() const;
// };
// using SymbolList = std::vector<Symbol>;
// auto make_empty_symbol_list() -> SymbolList;

// // 不行 对于一些常用的指针类型 可以使用using std::shared_ptr<Symbol>
// // three address code
// // 来自龙书 indirect triple
// struct Instruction {
//     int op;
//     Symbol left;
//     std::optional<Symbol> right;
//     Symbol result;
//     std::string to_string() const;
// };
// using InstructionPtr = std::shared_ptr<Instruction>;
// auto make_instruction(Instruction instruction) -> InstructionPtr;

// // 是在源代码中的位置
// struct Label {
//     std::string name;
//     std::string to_string() const;
// };
// using LabelPtr = std::shared_ptr<Label>;
// LabelPtr make_label(Label label);

// struct Branch {
//     std::optional<Symbol> condition;
//     std::string true_label;
//     std::optional<std::string> false_label;
//     std::string to_string() const;
// };
// using BranchPtr = std::shared_ptr<Branch>;
// BranchPtr make_branch(Branch branch);

// struct FnCall {
//     // 调用的函数名字
//     std::string name;
//     // 返回符号 但是可选
//     // std::optional<std::string> result;
//     // // 返回类型 可选
//     // std::optional<int> return_type;
//     // 参数列表在函数声明和函数定义中是一样的 可以定义一个typedef
//     // 因为后面会修改type的实现那
//     SymbolList parameter_list;
//     std::optional<int> return_type;
//     std::optional<Symbol> result;
//     std::string to_string() const;
// };
// using FnCallPtr = std::shared_ptr<FnCall>;
// FnCallPtr make_fncall(FnCall fncall);

// struct Return {
//     std::optional<Symbol> return_value;
//     std::string to_string() const;
// };
// using ReturnPtr = std::shared_ptr<Return>;
// ReturnPtr make_return(Return ret);

// struct SymbolDef {
//     bool local;
//     int type;
//     Symbol result;
//     std::optional<Symbol> size;
//     std::string to_string() const;
// };
// using SymbolDefPtr = std::shared_ptr<SymbolDef>;
// SymbolDefPtr make_symbol_def(SymbolDef symbol_def);

// struct Cast {
//     int cast;
//     Symbol value;
//     int type;
//     Symbol result;
//     std::string to_string() const;
// };
// using CastPtr = std::shared_ptr<Cast>;
// CastPtr make_cast(Cast cast);

// struct Gep {
//     Symbol value;
//     Symbol array_index;
//     Symbol struct_index;
//     Symbol result;
//     std::string to_string() const;
// };
// using GepPtr = std::shared_ptr<Gep>;
// GepPtr make_gep(Gep gep);

// using Code = std::variant<InstructionPtr, LabelPtr, BranchPtr, FnCallPtr,
//                           ReturnPtr, SymbolDefPtr, CastPtr, GepPtr>;
// using CodeList = std::vector<Code>;
// CodeList make_empty_code_list();

// // 这个实际上是定义 而不是声明
// struct FnDef {
//     // 函数名字
//     std::string name;
//     // 函数返回类型
//     // 参数列表
//     SymbolList parameter_list;
//     CodeList body;
//     bool varargs;
//     std::optional<int> return_type;
//     // 函数
//     std::string to_string() const;
// };
// using FnDefPtr = std::shared_ptr<FnDef>;
// FnDefPtr make_fndef(FnDef fndef);

// // todo
// struct SymbolDecl {
//     Symbol symbol;
//     std::string to_string() const;
// };
// using SymbolDeclPtr = std::shared_ptr<SymbolDecl>;
// SymbolDeclPtr make_symbol_decl(SymbolDecl symbol_decl);

// // 用来统一global 和 local

// struct StructDecl {
//     // type list
//     std::string name;
//     TypeList fields;
//     std::string to_string() const;
// };
// using StructDeclPtr = std::shared_ptr<StructDecl>;
// StructDeclPtr make_struct_decl(StructDecl struct_decl);

// using Decl = std::variant<FnDefPtr, StructDeclPtr, SymbolDefPtr,
// SymbolDeclPtr>; using DeclList = std::vector<Decl>; DeclList
// make_empty_decl_list();

// 实现一个map
// 对于表达式来说 操作和类型基本上是一一对应的 所以提供一个map就可以了

// type trait
// struct mov {};
// struct add {};

struct X86 {
    enum Coding {
        // instruction
        MOV,
        PUSH,
        POP,
        LEA,
        INC,
        DEC,
        NEG,
        NOT,
        ADD,
        SUB,
        IMUL,
        XOR,
        OR,
        AND,
        SAL,
        SHL,
        SAR,
        SHR,

        // data type size
        BYTE,   // 1 byte
        WORD,   // 2
        DWORD,  // 4
        QWORD,  // 8
        FLOAT,  // 4
        DOUBLE, // 8

        // addressing mode
        ABSOLUTE,
        INDIRECT,
        BASE_DISPLACEMENT,
        INDEXED,
        INDEXED_DISPLACEMENT,
    };
};

// 我们有很多这样的需求 经常需要将某个枚举值变为字符串
// 那么我们提供一个简单的数组做这个mapping不就可以了吗
// 这个mapping最好是作为一个函数提供 否则这个数组会被复制非常多份

std::string inst_to_string(int inst);

using ir::IR;
static std::unordered_map<int, int> mapping = {
    {IR::ADD, X86::ADD},
    // ...
};

} // namespace cgen
