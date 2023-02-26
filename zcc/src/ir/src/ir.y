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
    assign "="
    colon ":"
    left_brace "{"
    right_brace "}"
    comma ","
    left_parenthesis "("
    right_arenthesis ")"
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
%token <int> basic_type
// yy::parser::make_op(int, loc)
%token <int> OP "op"
// non terminal 就不会生成东西了, 现在type不表示任何东西 所以暂时用int代替
%token <std::string>
    GLOBAL_SYMBOL   "global_symbol"
    LOCAL_SYMBOL    "local_symbol"
    CONSTANT        "constant"

%nterm <ir::Instruction> instruction
%nterm <int> type
%nterm <std::string> symbol

%token <std::string>
    IDENTIFIER "identifier"

%nterm <ir::Code> code
%nterm <ir::CodeList> code_list

// only for test

%nterm <std::vector<ir::Instruction>> translation_unit instruction_list

%%

/* translation_unit
    :
    | translation_unit declaration {}
    | translation_unit definition {}
    | translation_unit type_alias {}
    ; */

// o 最终生成的东西可以放在driver里面呀 完美 drv
translation_unit
    : code_list {
        auto& code_list = drv.get_code_list();
        code_list = std::move($1);
    }
    ;

code_list
    : { $$ = ir::make_empty_code_list(); }
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
    | "identifier" ":" {
        $$ = ir::make_label($1);
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
    : basic_type {
        $$ = $1;
    }
    /* | struct_type {}
    | '*' type {} */
    ;

/* struct_type
    : STRUCT '{' type_list '}'
    ;

type_list
    :
    | type_list ',' type {}
    ;

parameter_list
    : {}
    | parameter_list ',' parameter {}
    ;

function_body
    : {}
    | function_body instruction {}
    ; */

 /* instruction
    : symbol "=" symbol ":" type "op" symbol ":" type {
        // 组织成一个instruction
        $$ = ir::make_binary_assignment($6, $3, $5, $7, $9, $1);
    }
    | "identifier" ":" {
        // label指示了一个位置 是当前的指令条数
        // 指令索引 对吧
        // 其实是下一条指令
        // 如果有两个连续的label我该怎么办呢??
        // 所以最好还是把label记录下来
        // 现在应该换一个新的类了 找一个新的基类
        // CodeLine
        // Instruction -> CodeLine
        // Label -> CodeLine
        // FnCall -> CodeLine
    }
    /*
    | call {}
    | assignment {}
    | branch {}
    | jump {}
    | return {} */


symbol
    : "global_symbol" { $$ = $1; }
    | "local_symbol" { $$ = $1; }
    | "constant" { $$ = $1; }
    ;

// 这个也包含 condition_assignment
/* binary_assignment
    : symbol '=' typed_symbol BINARY_OP typed_symbol {}
    ; */

/* unary_assignment
    : symbol '=' UNARY_OP typed_symbol {}
    ;

assignment
    : symbol '=' typed_symbol {}
    ;

branch
    : IF typed_symbol THEN label ELSE label {}
    ;

jump
    : JMP label {}
    ;

return
    : RET {}
    ;

label
    : ID ':' {}
    ;

// parameter 形参 在函数定义中出现
// argument 实参 在函数调用中出现
call
    : CALL GLOBAL_SYMBOL '(' argument_list ')' {}
    | symbol '=' CALL GLOBAL_SYMBOL '(' argument_list ')' ARROW type {}
    ;

*/
%%

// 必须自己实现一个error
void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
}