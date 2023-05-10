/*
 * 2023/5/10
 * zhangzhong
 * C Parser
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

%code requires {
// 本段代码被放在parser.hpp的开头
#include <string>
}

%code {

}

// 防止token命名冲突 加上前缀
%define api.token.prefix {C_TOKEN_}
%token 
    INT "int"
    RETURN "return"
    LEFT_BRACE "{"
    RIGHT_BRACE "}"
    LEFT_PARENTHESIS "("
    RIGHT_PARENTHESIS ")"
    SEMICOLON ";"
    
%token <std::string> ID
%token <std::string> NUM

%nterm <int> function_definition

%%

function_definition
    : "int" ID "(" ")" "{" "return" NUM ";" "}"
    ;

%%

// 必须自己实现一个error
void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
}