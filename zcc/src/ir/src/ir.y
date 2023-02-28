/*
 * 2023/2/24
 * zhangzhong
 * ir parser
 */

%skeleton "lalr1.cc"
%require "3.8.2"
%header
%locations

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

// 这一段代码放在 parser.hh
// 因为parser.hh里面用到了driver 所以必须在parser.hh里面前置声明
// 必须前置声明driver 因为parser需要使用
%code requires {
// 因为下面我们使用了std::string 所以在生成make_terminal的时候需要使用std::string
#include <string>
// 当然也得包括一些ir的定义 instruction address 等等
#include "ir.h"
    // 但是我们的driver在自己的命名空间里啊 能用上吗??
namespace ir { class driver; }
}
// 然后修改parser的参数，使之包含我们的driver
%param { ir::driver& drv }

// 然后我们包含parser.hh
// 这段代码会被放在parser.cc里面
%code {
#include "ir/driver.h"
}

// 防止token命名冲突 加上前缀
%define api.token.prefix {TOKEN_}
%token
    ASSIGN "="
    COLON ":"
    LEFT_BRACE "{"
    RIGHT_BRACE "}"
    COMMA ","
    LEFT_PARENTHESIS "("
    RIGHT_PARENTHESIS ")"
    ARROW "->"
    IF "if"
    THEN "then"
    ELSE "else"
    GLOBAL "global"
    LOCAL "local"
    FN "fn"
    STRUCT "struct"
    GOTO "goto"
    CALL "call"
    ASTERISK "*"
    RET "ret"
    NONE "none"
    ADDROF "addrof"
    LOAD "load"
    STORE "store"
    TO "to"
    GEP "gep"
    DECL "decl"
    ELLIPSIS "..."
    /* add "+"
    sub "-"
    mul "*"
    div "/"
    mod "%"
    bitand "&"
    bitor "|"
    bitxor "^"
    lsh "<<"
    rsh ">>"
    less "<"
    less_equal "<="
    great ">"
    great_equal ">="
    equal "=="
    not_equal "!=" */

    // basic type

// 如果我这么写了 他会生成什么呢
// yy::parser::make_basic_type(int, loc);
%token <int> BASIC_TYPE "basic_type"
// yy::parser::make_op(int, loc)
%token <int> OP "op"
%token <int> CAST "cast"
// non terminal 就不会生成东西了, 现在type不表示任何东西 所以暂时用int代替
%token <std::string>
    GLOBAL_SYMBOL   "global_symbol"
    LOCAL_SYMBOL    "local_symbol"
    CONSTANT        "constant"

%token <std::string>
    IDENTIFIER "identifier"

%nterm <ir::Code> code
%nterm <ir::CodeList> code_list
%nterm <ir::Symbol> symbol
%nterm <ir::SymbolList> parameter_list
%nterm <ir::Decl> decl
%nterm <ir::DeclList> decl_list
%nterm <ir::Type> type
%nterm <ir::TypeList> type_list
%nterm <std::string> name


%%
// o 最终生成的东西可以放在driver里面呀 完美 drv
translation_unit
    : decl_list {
        auto& decl_list = drv.get_decl_list();
        decl_list = std::move($1);
    }
    ;

decl_list
    : %empty { $$ = ir::make_empty_decl_list(); }
    | decl_list decl {
        $1.push_back($2);
        $$ = std::move($1);
    }
    ;

decl
    : "fn" name "(" parameter_list ")" "->" type "{" code_list "}" {
        $$ = ir::make_fndef({
            .name = $2,
            .parameter_list = $4,
            .body = $9,
            .return_type = $7,
        });
    }
    | "fn" name "(" parameter_list "," "..." ")" "->" type "{" code_list "}" {
        $$ = ir::make_fndef({
            .name = $2,
            .parameter_list = $4,
            .body = $11,
            .varargs = true,
            .return_type = $9,
        });
    }
    | "fn" name "(" parameter_list ")" "{" code_list "}" {
        $$ = ir::make_fndef({
            .name = $2, 
            .parameter_list = $4,
            .body = $7,
        });
    }
    | "fn" name "(" parameter_list "," "..." ")" "{" code_list "}" {
        $$ = ir::make_fndef({
            .name = $2, 
            .parameter_list = $4,
            .body = $9,
            .varargs = true,
        });
    }
    | "struct" "identifier" "{" type_list "}" {
        $$ = ir::make_struct_decl({
            .name = $2, 
            .fields = $4,
        });
    }
    | symbol "=" "global" type {
        $$ = ir::make_symbol_def({
            .type = $4,
            .result = $1,
        });
    }
    | symbol "=" "global" type "," symbol {
        $$ = ir::make_symbol_def({
            .type = $4,
            .result = $1,
            .size = $6,
        });
    }
    | "decl" symbol {
        $$ = ir::make_symbol_decl({
            .symbol = $2,
        });
    }
    ;

code_list
    : %empty { $$ = ir::make_empty_code_list(); }
    | code_list code {
        $1.push_back($2);
        $$ = std::move($1);
    }
    ;

code
    : symbol "=" symbol "op" symbol {
        // 组织成一个instruction
        $$ = ir::make_instruction({
            .op = $4,
            .left = $3,
            .right = $5,
            .result = $1,
        });
    }
    | symbol "=" symbol "*" symbol {
        $$ = ir::make_instruction({
            .op = ir::IR::MUL,
            .left = $3,
            .right = $5,
            .result = $1,
        });
    }
    | symbol "=" "addrof" symbol {
        $$ = ir::make_instruction({
            .op = ir::IR::ADDROF,
            .left = $4,
            .result = $1,
        });
    }
    | symbol "=" symbol {
        $$ = ir::make_instruction({
            .op = ir::IR::ASSIGN,
            .left = $3,
            .result = $1,
        });
    }
    | symbol "=" "load" symbol {
        $$ = ir::make_instruction({
            .op = ir::IR::LOAD,
            .left = $4,
            .result = $1,
        });
    }
    | "store" symbol "to" symbol {
        $$ = ir::make_instruction({
            .op = ir::IR::STORE,
            .left = $2,
            .result = $4,
        });
    }
    | "identifier" ":" {
        $$ = ir::make_label({.name = $1});
    }
    | "if" symbol "then" name "else" name {
        $$ = ir::make_branch({
            .condition = $2,
            .true_label = $4,
            .false_label = $6,
        });
    } 
    | "if" symbol "then" name {
        $$ = ir::make_branch({.condition = $2, .true_label = $4});
    }
    | "goto" name {
        $$ = ir::make_branch({.true_label = $2});
    }
    | "call" name "(" parameter_list ")" {
        $$ = ir::make_fncall({.name = $2, .parameter_list = $4});
    }
    | symbol "=" "call" name "(" parameter_list ")" "->" type {
        $$ = ir::make_fncall({
            .name = $4,
            .parameter_list = $6,
            .return_type = $9,
            .result = $1,
        });
    }
    | "ret" "none" {
        $$ = ir::make_return({});
    } 
    | "ret" symbol {
        $$ = ir::make_return({.return_value = $2});
    }
    | symbol "=" "cast" symbol "to" type {
        $$ = ir::make_cast({
            .cast = $3,
            .value = $4,
            .type = $6,
            .result = $1,
        });
    }
    | symbol "=" "local" type {
        $$ = ir::make_symbol_def({
            .local = true,
            .type = $4,
            .result = $1,
        });
    }
    | symbol "=" "local" type "," symbol {
        $$ = ir::make_symbol_def({
            .local = true,
            .type = $4,
            .result = $1,
            .size = $6,
        });
    }
    | symbol "=" "gep" symbol "," symbol "," symbol {
        $$ = ir::make_gep({
            .value = $4,
            .array_index = $6,
            .struct_index = $8,
            .result = $1,
        });
    }
    ;

parameter_list
    : %empty { 
        // 创建一个parameterlist
        $$ = ir::make_empty_symbol_list();
    }
    | symbol {
        auto list = ir::make_empty_symbol_list();
        list.push_back($1);
        $$ = std::move(list);
    }
    | parameter_list "," symbol {
        $1.push_back($3);
        $$ = std::move($1);
    }
    ;

type_list
    : %empty { $$ = ir::make_empty_type_list(); }
    | type {
        auto type_list = ir::make_empty_type_list();
        type_list.push_back($1);
        $$ = type_list;
    }
    | type_list "," type {
        $1.push_back($3);
        $$ = $1;
    }
    ;

type
    : "basic_type" {
        // $$ = $1;
        $$ = ir::make_basic_type($1);
        // std::cout << "rule basic type\n";
    }
    | "*" type {
        $$ = ir::make_pointer_type($2);
        // std::cout << "rule pointer type\n";
    }
    | "fn" "(" type_list ")" {
        $$ = ir::make_fn_type({.parameter_type = $3});
    }
    | "fn" "(" type_list ")" "->" type {
        $$ = ir::make_fn_type({.parameter_type = $3, .return_type = $6});
    }
    | "fn" "(" type_list "," "..." ")" {
        $$ = ir::make_fn_type({
            .parameter_type = $3,
            .varargs = true,
        });
    }
    | "fn" "(" type_list "," "..." ")" "->" type {
        $$ = ir::make_fn_type({
            .parameter_type = $3, 
            .varargs = true,
            .return_type = $8,
        });
    }
    | "identifier" {
        $$ = ir::make_struct_type($1);
    }
    ;

symbol
    : name ":" type {
        $$ = ir::Symbol{.name = $1, .type = $3};
    }
    ;

name
    : "global_symbol" { $$ = $1; }
    | "local_symbol" { $$ = $1; }
    | "constant" { $$ = $1; }
    ;
%%

// 必须自己实现一个error
void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
}
