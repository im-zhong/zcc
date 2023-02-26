/*
 * 2023/2/24
 * zhangzhong
 * IR scanner
 */

%option noyywrap nounput noinput nodefault batch

%{
#include "ir/driver.h"
#include "ir/parser.hpp"
using ir::IR;
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)
NUM ([0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?)
/* idenfitier */
ID [a-z0-9][a-z0-9]*

blank [ \t\r]
%{
    // code run each time a pattern is matched
    #define YY_USER_ACTION loc.columns(yyleng);
%}
%%
%{
    // code run each time yylex is called
    yy::location& loc = drv.get_location();
    // step over match word, begin = end
    loc.step();    
%}
{blank}+    { loc.step(); }
\n+         { loc.lines(yyleng); loc.step(); }
 /* annotation */
#.*\n       { loc.lines(); loc.step(); 
    // 我懂了 因为我们不会return 所以必须主动loc.step()
    // 如果我们的action会return的话 那么就不需要调用 loc.step 了
}

 /* keyword */
 /* "global" { return GLOBAL; }
 "local" { return LOCAL; }
 "if" { return IF; }
 "then" { return THEN; }
 "else" { return ELSE; }
 "decl" { return DECL; }
 "fn" { return FN; }
 "struct" { return STRUCT; }
 "type" { return TYPE; } */

 /* basic type C++的枚举没法自动转换成 */
"bool"  { return yy::parser::make_basic_type(IR::BOOL, loc); }
"i8"    { return yy::parser::make_basic_type(IR::I8, loc); }
"i16"   { return yy::parser::make_basic_type(IR::I16, loc); }
"i32"   { return yy::parser::make_basic_type(IR::I32, loc); }
"i64"   { return yy::parser::make_basic_type(IR::I64, loc); }
"f32"   { return yy::parser::make_basic_type(IR::F32, loc); }
"f64"   { return yy::parser::make_basic_type(IR::F64, loc); }

 /* seperator */
":"     { return yy::parser::make_colon(loc); }
"{"     { return yy::parser::make_left_brace(loc); }
"}"     { return yy::parser::make_right_brace(loc); }
","     { return yy::parser::make_comma(loc); }
"="     { return yy::parser::make_assign(loc); }
"("     { return yy::parser::make_left_parenthesis(loc); }
")"     { return yy::parser::make_right_arenthesis(loc); }

 /* cast */
 /* "bitcast" {}
 "to" {}
 "load" {}
 "store" {}
 "zeroext" {}
 "signext" {}
 "trunc" {}
 "fext" {}
 "ftrunc" {}
 "ptrtoint" {}
 "inttoptr" {} */
                                             
 /* global identifier */
"@"{ID} {
    // 暂时用string代替string_table
    // 而且目前测试没有yylval
    return yy::parser::make_GLOBAL_SYMBOL(yytext, loc);
}

 /* local, temproray identifier */
"%"{ID} {
    return yy::parser::make_LOCAL_SYMBOL(yytext, loc);
}

 /* constant number */
"$"{NUM} {
    return yy::parser::make_CONSTANT(yytext, loc);
}

 /* binary operator; compare operator */
"+"     { return yy::parser::make_OP(IR::ADD, loc); }
"-"     { return yy::parser::make_OP(IR::SUB, loc); }
"*"     { return yy::parser::make_OP(IR::MUL, loc); }
"/"     { return yy::parser::make_OP(IR::DIV, loc); }
"%"     { return yy::parser::make_OP(IR::MOD, loc); }
"&"     { return yy::parser::make_OP(IR::BITAND, loc); }
"|"     { return yy::parser::make_OP(IR::BITOR, loc); }
"^"     { return yy::parser::make_OP(IR::BITXOR, loc); }
">>"    { return yy::parser::make_OP(IR::RSH, loc); }
"<<"    { return yy::parser::make_OP(IR::LSH, loc); }
"<"     { return yy::parser::make_OP(IR::LESS, loc); }
"<="    { return yy::parser::make_OP(IR::LESS_QEUAL, loc); }
">"     { return yy::parser::make_OP(IR::GREAT, loc); }
">="    { return yy::parser::make_OP(IR::GREAT_EQUAL, loc); }
"=="    { return yy::parser::make_OP(IR::EQUAL, loc); }
"!="    { return yy::parser::make_OP(IR::NOT_EQUAL, loc); }

 /* string literal */

<<EOF>>    { return yy::parser::make_YYEOF(loc); }

.   {
    throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
}

%%

void ir::driver::begin_scan() {
    if (filename.empty() || filename == "-") {
        yyin = stdin;
    } else if (!(yyin = fopen(filename.c_str(), "r"))) {
        std::cerr << "can not open " << filename << ": " << strerror(errno) << std::endl;
    }
}

void ir::driver::end_scan() {
    fclose(yyin);
}
