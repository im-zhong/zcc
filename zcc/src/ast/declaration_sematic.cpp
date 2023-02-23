#include "ast/declaration_sematic.h"
#include "ast/syntax_error.h"
#include "core/grammar_abstract.h"
// #include "util/table.h"
#include "type/type.h"

_CML_BEGIN_

DeclQualifierType DeclarationSpecifierSemantic::compute() {
    // 设置默认类型为int/
    // 如果已经设置则不会生效
    set_type(TERMINAL::RES_INT);
    DeclQualifierType qualifier_type(declaration_type);

    if (check_sematic()) {
        if (type == TERMINAL::RES_CHAR) {
            declaration_type = char_type;
        } else if (type == TERMINAL::RES_FLOAT) {
            declaration_type = float_type;
        } else if (type == TERMINAL::RES_DOUBLE) {
            declaration_type = double_type;
        } else if (size == TERMINAL::RES_SHORT) {
            declaration_type = short_type;
        } else if (size == TERMINAL::RES_LONG) {
            // long double = double 哈哈 偷懒了
            if (type == TERMINAL::RES_INT)
                declaration_type = long_type;
            else if (type == TERMINAL::RES_DOUBLE)
                declaration_type = double_type;
        }

        if (sign)
            qualifier_type.make_signed();
        else
            qualifier_type.make_unsigned();

        if (const_qualifier)
            qualifier_type.make_const();
        if (volatile_qualifier)
            qualifier_type.make_volatile();
    } else {
        syntax_error("错误的声明格式");
    }

    return qualifier_type;
}

bool DeclarationSpecifierSemantic::check_sematic() {
    bool b = (size == TERMINAL::RES_SHORT && type != TERMINAL::RES_INT) ||
             (size == TERMINAL::RES_LONG && type != TERMINAL::RES_INT &&
              type != TERMINAL::RES_DOUBLE) ||
             (sign && type != TERMINAL::RES_INT && type != TERMINAL::RES_CHAR);
    return !b;
}

// 暂时不实现这个功能
std::shared_ptr<Type>
DeclarationSpecifierSemantic::is_typename(const std::string& type_lexeme) {
    return std::shared_ptr<Type>(nullptr);
}

bool DeclarationSpecifierSemantic::set_type(std::shared_ptr<Type> type) {
    if (type && !is_type) {
        declaration_type = type;
        is_type = true;
        return true;
    } else
        return false;
}

// 提供默认定义
DeclarationSpecifierSemantic make_declaration_specifier_semantic() {
    return DeclarationSpecifierSemantic(0, false, false, false, 0, 0);
}

// 能提供什么样的信息呢
// type identifier initializer
// 应该可以提供这三方面的信息
void DeclarationSemantic::compute() {}

_CML_END_
