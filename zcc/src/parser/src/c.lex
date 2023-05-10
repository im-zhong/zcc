/*
 * 2023/5/10
 * zhangzhong
 * C Scanner
 */

%option noyywrap nounput noinput nodefault batch

%{
#include "parser.hpp"
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)
NUM ([0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?)
/* idenfitier */
ID [_A-Za-z][_A-Za-z0-9]*
/* 注意这里没有\n 因为\n需要单独处理 来正确得到token的位置 */
BLANK [ \t\r]

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

{BLANK}+    { loc.step(); }
\n+         { loc.lines(yyleng); loc.step(); }

 /* annotation */
#.*\n       { loc.lines(); loc.step(); 
    // 我懂了 因为我们不会return 所以必须主动loc.step()
    // 如果我们的action会return的话 那么就不需要调用 loc.step 了
}

<<EOF>>     { return yy::parser::make_YYEOF(loc); }
. {
    throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
}


 /* keywords */
"int"       {}
"return"    {}

 /* seperators */
"("         {}
")"         {}
"{"         {}
"}"         {}
";"        {}

    /* operators */

    /* number */
{NUM}   {}



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
