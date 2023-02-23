//
// Copyright (c) 2019.03
// Zhong
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.
//

#ifndef _ZCC_GRAMMAR_ABSTRACT_H_
#define _ZCC_GRAMMAR_ABSTRACT_H_

#include "core.h"
#include <map>
#include <string>

_CML_BEGIN_ // namespace cml begin

    enum NONTERMINAL : uint8_t {
        TRANSLATION_UNIT,           // translation_unit
        EXTERNAL_DECL,              // external_decl
        FUNCTION_DEFINITION,        // function_definition
        DECL,                       // decl
        DECL_LIST,                  // decl_list
        DECL_SPECS,                 // decl_specs
        STORAGE_CLASS_SPEC,         // storage_class_spec
        TYPE_SPEC,                  // type_spec
        TYPE_QUALIFIER,             // type_qualifier
        STRUCT_OR_UNION_SPEC,       // struct_or_union_spec
        STRUCT_OR_UNION,            // struct_or_union
        STRUCT_DECL_LIST,           // struct_decl_list
        INIT_DECLARATOR_LIST,       // init_declarator_list
        INIT_DECLARATOR,            // init_declarator
        STRUCT_DECL,                // struct_decl
        SPEC_QUALIFIER_LIST,        // spec_qualifier_list
        STRUCT_DECLARATOR_LIST,     // struct_declarator_list
        STRUCT_DECLARATOR,          // struct_declarator
        ENUM_SPEC,                  // enum_spec
        ENUMERATOR_LIST,            // enumrator_list
        ENUMERATOR,                 // enumerator
        DECLARATOR,                 // declarator
        DIRECT_DECLARATOR,          // direct_declarator
        POINTER,                    // pointer
        TYPE_QUALIFIER_LIST,        // type_qualifier_list
        PARAM_TYPE_LIST,            // param_type_list
        PARAM_LIST,                 // param_list
        PARAM_DECL,                 // param_decl
        ID_LIST,                    // id_list
        INITIALIZER,                // initializer
        INITIALIZER_LIST,           // initializer_list
        TYPE_NAME,                  // type_name
        ABSTRACT_DECLARATOR,        // abstract_declarator
        DIRECT_ABSTRACT_DECLARATOR, // direct_abstract_declarator
        TYPEDEF_NAME,               // typedef_name
        STAT,                       // stat
        LABELED_STAT,               // labeled_stat
        EXP_STAT,                   // exp_stat
        COMPOUND_STAT,              // compound_stat
        STAT_LIST,                  // stat_list
        SELECTION_STAT,             // selection_stat
        ITERATION_STAT,             // iteration_stat
        JUMP_STAT,                  // jump_stat
        EXP,                        // exp
        ASSIGNMENT_EXP,             // assignment_exp
        ASSIGNMENT_OPERATOR,        // assignment_operator
        CONDITIONAL_EXP,            // conditional_exp
        CONST_EXP,                  // const_exp
        LOGICAL_OR_EXP,             // logical_or_exp
        LOGICAL_AND_EXP,            // logical_and_exp
        INCLUSIVE_OR_EXP,           // inclusive_or_exp
        EXCLUSIVE_OR_EXP,           // exclusive_or_exp
        AND_EXP,                    // and_exp
        EQUALITY_EXP,               // equality_exp
        RELATIONAL_EXP,             // relational_exp
        SHIFT_EXPRESSION,           // shift_expression
        ADDITIVE_EXP,               // additive_exp
        MULT_EXP,                   // mult_exp
        CAST_EXP,                   // cast_exp
        UNARY_EXP,                  // unary_exp
        UNARY_OPERATOR,             // unary_operator
        POSTFIX_EXP,                // postfix_exp
        PRIMARY_EXP,                // primary_exp
        ARGUMENT_EXP_LIST,          // argument_exp_list
        CONST,                      // const
    };                              // NONTERMINAL

enum TERMINAL : uint8_t {
    ID, // identifer

    OP_INC,  // ++ (increment)
    OP_DEC,  // -- (decrement)
    OP_ADD,  // +  (plus)
    OP_SUB,  // -  (substract)
    OP_MUL,  // *  (multi)
    OP_DIV,  // /  (division)
    OP_MOD,  // %  (mod)
    OP_ADDA, // += (plus assgin)
    OP_SUBA, // -= (substract assgin)
    OP_MULA, // *= (multi assgin)
    OP_DIVA, // /= (division assgin)
    OP_MODA, // %= (mod assgin)

    OP_AND,   // &  (bit and)
    OP_OR,    // |  (or)
    OP_NOT,   // ~  (not)
    OP_XOR,   // ^  (xor)
    OP_SHL,   // << (left shift)
    OP_SHR,   // >> (right shift)
    OP_BANDA, // &= (bit and equal)
    OP_ORA,   // |= (or assgin)
    OP_XORA,  // ^= (xor assgin)
    OP_SHLA,  // <<=(left shift assgin)
    OP_SHRA,  // >>=(right shift assgin)

    OP_GRE,  // >  (greater than)
    OP_GREE, // >= (greater than equal)
    OP_LES,  // <  (less than)
    OP_LESE, // <= (less than equal)
    OP_EQU,  // == (equal)
    OP_NEQ,  // != (not equal)

    OP_LNOT, // !  (logical not)
    OP_LAND, // && (logical and)
    OP_LOR,  // || (logical or)

    OP_ASN, // =  (assgin)
    OP_PER, // .  (period)
    OP_ARR, // -> (arrow)
    OP_COL, // :  (colon)
    OP_QUE, // ?  (question)
    OP_ELL, // ...(ellipsis)

    DLM_COMMA, // ,  (comma)
    DLM_SEMIC, // ;  (semicolon)
    DLM_SOE,   // (  (left circle bracket, start of exp)
    DLM_EOE,   // )  (right circle bracket, end of exp)
    DLM_SOCE,  // [  (left square bracket, start of const exp)
    DLM_EOCE,  // ]  (right square bracket, end of const exp)
    DLM_SOS,   // {  (left bracket, start of scope)
    DLM_EOS,   // }  (right bracket, end of scope)

    RES_AUTO,     // auto
    RES_BREAK,    // break
    RES_CASE,     // case
    RES_CHAR,     // char
    RES_CONST,    // const
    RES_CONTINUE, // continue
    RES_DEFAULT,  // default
    RES_DO,       // do
    RES_DOUBLE,   // double
    RES_ELSE,     // else
    RES_ENUM,     // enum
    RES_EXTERN,   // extern
    RES_FLOAT,    // float
    RES_FOR,      // for
    RES_GOTO,     // goto
    RES_IF,       // if
    RES_INT,      // int
    RES_LONG,     // long
    RES_REGISTER, // register
    RES_RETURN,   // return
    RES_SHORT,    // short
    RES_SIGNED,   // signed
    RES_SIZEOF,   // sizeof
    RES_STATIC,   // static
    RES_STRUCT,   // struct
    RES_SWITCH,   // switch
    RES_TYPEDEF,  // typedef
    RES_UNION,    // union
    RES_UNSIGNED, // unsigned
    RES_VOID,     // void
    RES_VOLATILE, // volatile
    RES_WHILE,    // while

    LIT_STR,  // literal string
    LIT_CHAR, // literal char
    LIT_ENUM, // literal enumeration
    LIT_REAL, // literal real
    LIT_INT,  // literal integer
};            // TERMINAL

inline uint8_t check_reserved(const std::string& lexeme) {
    static std::map<std::string, TERMINAL> reserved_word = {
        {"auto", TERMINAL::RES_AUTO},
    };
    auto it = reserved_word.find(lexeme);
    if (it != reserved_word.end()) {
        return it->second;
    }
    return -1;
}

constexpr int NONTERMINAL_SIZE = 65;
constexpr int TERMINAL_SIZE = 84;

_CML_END_ // namespace cml end

#endif // _ZCC_GRAMMAR_ABSTRACT_H_