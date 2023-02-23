#pragma once

/*
        语法分析与语义计算实现模式

        语义计算单元提供component
        语法分析分析token流填充component并初步检查不符合语法的错误
        然后语义计算单元使用component中提供的信息进行语义计算并检查语义错误
        这样语法分析和语义分析就是分开写的
        语法分析专注于结构
        语义分析专注于语义计算
        对于我这个新手来说相对简单一点

        TODO 完善符号表和类型表的实现 暂停语义模块的实现
        2020/3/24
        类型系统的设计还是有问题
        我把const从类型中独立出来非常不利于实现
        因为可能存在 void func(const int);
        这样的类型存在
        这样cosnt必须依附在一个类型上 不能依附在一个symbol上
        所以现在的类型系统设计是错误的
        所以在现存的类型系统上添加ConstType VolatileType
        并且添加相应的标志位 这样容易检测是否为cosnt volatile
*/

#ifndef _ZCC_PARSER_H_
#define _ZCC_PARSER_H_

#include "scan/lexer.h"
// #include "declaration.h" // 因为这个类中存在同名函数 所以在重构之前不能包含
// #include "expression"
// #include "statement"

#include "declaration_sematic.h"
#include "scan/token.h"
#include <map>
#include <set>
#include <string>
#include <vector>

_CML_BEGIN_

class Parser {
  public:
    Parser() { init(); }

    void load(const std::string& filename) {
        if (lexer.load(filename)) {

        } else {
            std::cerr << "文件读取错误" << std::endl;
        }
    }

    void do_parser();

    std::vector<Token> token_vector;
    int token_index = 0;

    Token next_token() {
        // 在做测试的时候先分离实现

        Token token = lexer.lexer();
        std::cout << token << std::endl;
        return token;
        // if (token_index < token_vector.size())
        //{
        //	++token_index;
        //	return token_vector[token_index - 1];
        // }
        // else
        //{
        //	return { static_cast<uint8_t>(-1) , "" };
        // }
    }

    void enter_scope() { ++scope; }
    void exit_scope() { --scope; }

  private:
    // 有很多很多这样的函数 其实不如做成一张表更划算
    bool is_external_declaration(const Token& token);
    bool is_storage_class_specifier(const Token& token);
    bool is_type_specifier(const Token& token);
    bool is_type_qualifier(const Token& token);
    bool is_init_declarator(const Token& token);
    bool is_declaration_specifiers(const Token& token);
    bool is_fields(const Token& token);
    bool is_declarator(const Token& token);
    bool is_abstract_declarator(const Token& token);

    std::set<std::string> type_specifier_table;
    std::set<std::string> type_qualifier_table;
    std::set<std::string> storage_class_specifier_table;

    std::map<std::string, std::set<std::string>> first_table;
    std::map<std::string, std::set<std::string>> follow_table;
    std::map<std::string, std::set<std::string>> augment_first_table;

    void init();

    void translation_unit();
    void external_declaration();
    void function_definition();
    void declaration();
    void init_declarator();
    void initializer();
    void declarator();
    void pointer();
    void direct_declarator();
    void suffix_declarator();
    // 语义计算
    void array_suffix(DeclQualifierType type);
    void function_suffix(DeclQualifierType type);

    void parameter_list();
    void parameter();
    DeclQualifierType declaration_specifiers();
    void storage_class_specifier();
    void type_specifier();
    void typedef_name();
    void type_qualifier();
    void struct_or_union_specifier();
    void struct_or_union();
    void fields();
    void field();
    void enum_specifier();
    void enumerator();
    void type_name();
    void abstract_declarator();
    void suffix_abstract_declarator();

    Token token;
    Lexer lexer;

    // 语义分析需要的数据
    int scope = 0;
};

_CML_END_
#endif // _ZCC_PARSER_H_