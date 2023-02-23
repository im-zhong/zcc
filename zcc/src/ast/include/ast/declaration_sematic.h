#pragma once

#ifndef _ZCC_DECLARATION_SEMATIC_H_
#define _ZCC_DECLARATION_SEMATIC_H_

#include "core/grammar_abstract.h"
#include "symbol/symbol.h"
#include "type/type.h"
#include <memory>
#include <stack>

_CML_BEGIN_

// 3/24
// 添加const volatile修饰
// 这个类型只是一个补丁 没有实际的意义
class DeclQualifierType {

  public:
    DeclQualifierType(std::shared_ptr<Type> type) : type(type) {}
    void make_const() { const_qualifier = true; }
    void make_volatile() { volatile_qualifier = true; }
    void make_signed() { sign = true; }
    void make_unsigned() { sign = false; }

    std::shared_ptr<Type> type;
    bool const_qualifier = false;
    bool volatile_qualifier = false;
    bool sign = true; // 默认应该是带符号的
};

struct DeclarationSpecifierSemantic {
    DeclarationSpecifierSemantic(uint8_t storage_class, bool const_qualifier,
                                 bool volatile_qualifier, bool sign,
                                 uint8_t size, uint8_t type)
        : storage_class(storage_class), const_qualifier(const_qualifier),
          volatile_qualifier(volatile_qualifier), sign(sign), size(size),
          type(type) {
        declaration_type = int_type;
    }

    // function
    DeclQualifierType compute();

    bool check_sematic();

    std::shared_ptr<Type> is_typename(const std::string& type_lexeme);

    // set
    bool set_storage_class(uint8_t storage_class) {
        if (!is_storage_class) {
            this->storage_class = storage_class;
            is_storage_class = true;
            return true;
        } else
            return false;
    }
    bool set_const_qualifier(bool const_qualifier) {
        if (!is_const_qualifier) {
            this->const_qualifier = const_qualifier;
            is_const_qualifier = true;
            return true;
        } else
            return false;
    }
    bool set_volatile_qualifier(bool volatile_qualifier) {
        if (!is_volatile_qualifier) {
            this->volatile_qualifier = volatile_qualifier;
            is_volatile_qualifier = true;
            return true;
        } else
            return false;
    }
    bool set_sign(bool sign) {
        if (!is_sign) {
            this->sign = sign;
            is_sign = true;
            return true;
        } else
            return false;
    }
    bool set_size(uint8_t size) {
        if (!is_size) {
            this->size = size;
            is_size = true;
            return true;
        } else
            return false;
    }
    bool set_type(uint8_t type) {
        if (!is_type) {
            this->type = type;
            is_type = true;
            return true;
        } else
            return false;
    }
    // special set
    bool set_type(std::shared_ptr<Type> type);

    // get
    uint8_t get_storage_class() const noexcept { return storage_class; }
    bool get_const_qualifier() const noexcept { return const_qualifier; }
    bool get_volatile_qualifier() const noexcept { return volatile_qualifier; }
    bool get_sign() const noexcept { return sign; }
    uint8_t get_size() const noexcept { return size; }
    uint8_t get_type() const noexcept { return type; }

  protected:
    uint8_t storage_class;
    bool const_qualifier;
    bool volatile_qualifier;
    bool sign;
    uint8_t size;
    uint8_t type;

    // 表示当前值是否被设置
    bool is_storage_class = false;
    bool is_const_qualifier = false;
    bool is_volatile_qualifier = false;
    bool is_sign = false;
    bool is_size = false;
    bool is_type = false;

    std::shared_ptr<Type> declaration_type;
};

// 提供默认定义
DeclarationSpecifierSemantic make_declaration_specifier_semantic();

// declarator语义
class DeclaratorSemantic {
  public:
    std::stack<TERMINAL> reverse_type;
};

class DeclarationSemantic {
  public:
    void compute();

    std::shared_ptr<Symbol> declaration_global(uint8_t storage_class,
                                               const std::string& identifier,
                                               std::shared_ptr<Type> type,
                                               Coordinate coordinate);

    // 语义函数
    std::shared_ptr<Symbol> declare_symbol();
};

// 还得有一个分析初始化值的语义函数
// 不过现在还写不了
class InitializerSemantic {
  public:
};

_CML_END_

#endif // _ZCC_DECLARATION_SEMATIC_H_