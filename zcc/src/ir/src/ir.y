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
// non terminal 就不会生成东西了, 现在type不表示任何东西 所以暂时用int代替
%token <std::string>
    GLOBAL_SYMBOL   "global_symbol"
    LOCAL_SYMBOL    "local_symbol"
    CONSTANT        "constant"

%nterm <std::string> symbol

%token <std::string>
    IDENTIFIER "identifier"

%nterm <ir::Code> code
%nterm <ir::CodeList> code_list
%nterm <ir::SymbolList> parameter_list
%nterm <ir::Decl> decl
%nterm <ir::DeclList> decl_list
%nterm <ir::Type> type
%nterm <ir::TypeList> type_list

%%
// o 最终生成的东西可以放在driver里面呀 完美 drv
translation_unit
    : decl_list {
        auto& decl_list = drv.get_decl_list();
        decl_list = std::move($1);
    }
    ;

decl
    : "fn" symbol "(" parameter_list ")" "->" type "{" code_list "}" {
        $$ = ir::make_fndecl($2, $4, $7, $9);
    }
    | "fn" symbol "(" parameter_list ")" "{" code_list "}" {
        $$ = ir::make_noret_fndecl($2, $4, $7);
    }
    | "struct" "identifier" "{" type_list "}" {
        $$ = ir::make_struct_decl($2, $4);
    }
    | "global" symbol "=" symbol ":" type {
        $$ = ir::make_global_decl($2, $4, $6);
    }
    ;

decl_list
    : %empty { $$ = ir::make_empty_decl_list(); }
    | decl_list decl {
        $1.push_back($2);
        $$ = std::move($1);
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
    : symbol "=" symbol ":" type "op" symbol ":" type {
        // 组织成一个instruction
        $$ = ir::make_binary_assignment($6, $3, $5, $7, $9, $1);
    }
    | symbol "=" symbol ":" type "*" symbol ":" type {
        $$ = ir::make_binary_assignment(ir::IR::MUL, $3, $5, $7, $9, $1);
    }
    | symbol "=" "addrof" symbol ":" type {
        $$ = ir::make_addrof($1, $4, $6);
    }
    | "identifier" ":" {
        $$ = ir::make_label($1);
    }
    | symbol "=" symbol ":" type {
        $$ = ir::make_assignment($3, $5, $1);
    }
    | "if" symbol ":" type "then" symbol "else" symbol {
        $$ = ir::make_full_branch($2, $4, $6, $8);
    } 
    | "if" symbol ":" type "then" symbol {
        $$ = ir::make_half_branch($2, $4, $6);
    }
    | "goto" symbol {
        $$ = ir::make_goto($2);
    }
    | "call" symbol "(" parameter_list ")" {
        $$ = ir::make_fncall($2, $4);
    }
    | symbol "=" "call" symbol "(" parameter_list ")" "->" type {
        $$ = ir::make_fncall_assignment($4, $6, $1, $9);
    }
    | "ret" "none" {
        $$ = ir::make_return();
    } 
    | "ret" symbol ":" type {
        $$ = ir::make_return(ir::TypedSymbol{.symbol = $2, .type = $4});
    }
    | symbol "=" "local" type {
        $$ = ir::make_local_decl($1, $4);
    }
    | symbol "=" "load" symbol ":" type {
        $$ = ir::make_load($1, $4, $6);
    }
    | "store" symbol ":" type "to" symbol ":" type {
        $$ = ir::make_store($2, $4, $6, $8);
    }
    ;

parameter_list
    : %empty { 
        // 创建一个parameterlist
        $$ = ir::make_empty_symbol_list();
    }
    | symbol ":" type {
        auto list = ir::make_empty_symbol_list();
        list.push_back(ir::TypedSymbol{.symbol = $1, .type = $3});
        $$ = std::move(list);
    }
    | parameter_list "," symbol ":" type {
        $1.push_back(ir::TypedSymbol{.symbol = $3, .type = $5});
        $$ = std::move($1);
    }
    ;

/* type_alias
    : TYPE TYPE_ID '=' type {}
    ; */

/* declaration
    : DECL FN GLOBAL_SYMBOL '(' type_list ')' ARROW type {}
    | GLOBAL GLOBAL_SYMBOL '=' typed_constant {}
    | CONST GLOBAL_SYMBOL '=' typed_constant {}
    ; */

/* definition
    : FN GLOBAL_SYMBOL '(' parameter_list ')' ARROW type '{' function_body '}' {}
    ; */

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
        $$ = ir::make_fn_type($3);
    }
    | "fn" "(" type_list ")" "->" type {
        $$ = ir::make_fn_type($3, $6);
    }
    | "identifier" {
        $$ = ir::make_struct_type($1);
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

symbol
    : "global_symbol" { $$ = $1; }
    | "local_symbol" { $$ = $1; }
    | "constant" { $$ = $1; }
    ;
%%

// 必须自己实现一个error
void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
}