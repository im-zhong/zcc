/*
        aggregate_type.h
        zhong, ysu
*/

#ifndef _ZCC_AGGREGATE_TYPE_H_
#define _ZCC_AGGREGATE_TYPE_H_

#include "primitive_type.h"
#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

// enum struct union 都有一个叫做 域 的东西
// field
// 所谓的域其实就比Symbol多了一个offset
// 我们可以用相同的方法创建一个域
struct Field {
    Field(const std::shared_ptr<Symbol>& symbol, int offset)
        : symbol(symbol), offset(offset) {}
    std::shared_ptr<Symbol> symbol;
    int offset; // offset是该符号在整个Field中以字节为单位的偏移量
};

std::shared_ptr<Field> make_field(const std::shared_ptr<Symbol>& symbol,
                                  int offset);

class AggregateType : public Type {
  public:
    AggregateType(uint32_t type_word, const std::string& name)
        : Type(type_word, 0, 0), name(name) {}

    // AggregateType(uint32_t type_word, const std::string& name, const
    // std::list<std::shared_ptr<Symbol>>& field_symbols) : 	Type(type_word,
    // 0,
    // 0), name(name)
    //{
    //	insert(field_symbols);
    // }

    std::shared_ptr<Symbol> find(const std::string& name) {
        for (const auto& field : fields) {
            if (field->symbol->name == name)
                return field->symbol;
        }
        return nullptr;
    }

    std::shared_ptr<Symbol> find(int offset) {
        for (const auto& field : fields) {
            if (field->offset == offset)
                return field->symbol;
        }
        return nullptr;
    }

    virtual bool insert(const std::shared_ptr<Symbol>& symbol) = 0;
    // TODO 如果在插入列表中有同名符号 应该怎么做 是跳过还是直接报错
    virtual bool
    insert(const std::list<std::shared_ptr<Symbol>>& field_symbols) = 0;

    std::string get_name() const noexcept { return name; }
    const std::list<std::shared_ptr<Field>>& get_fields() const noexcept {
        return fields;
    }

    bool is_defined() const noexcept { return defined; }

  protected:
    virtual bool check_definition(const std::string& name) const noexcept = 0;

  protected:
    std::list<std::shared_ptr<Field>> fields{};
    std::string name{};
    bool defined{}; // 检查类型是否重复定义 ??
};

_CML_END_

#endif // _ZCC_AGGREGATE_TYPE_H_