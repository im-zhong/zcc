#pragma once

#ifndef _ZCC_QUALIFIER_TYPE_H_
#define _ZCC_QUALIFIER_TYPE_H_

#include "primitive_type.h"
_CML_BEGIN_

class QualifierType : public Type {
  public:
    QualifierType(const std::shared_ptr<Type>& type, uint32_t qualifier_word)
        : Type(qualifier_word, type->get_size()), inner_type(type) {}

    std::shared_ptr<Type> get_inner_type() const noexcept { return inner_type; }

    virtual std::string to_string() const noexcept override;

  private:
    std::shared_ptr<Type> inner_type;
};

// 类型系统实现的正确性不依赖与本系统 而依赖于语义检查
// 这样虽然不符合软件工程 但是可以让实现变成简单自然
std::shared_ptr<QualifierType> make_const(std::shared_ptr<Type> type);
std::shared_ptr<QualifierType> make_volatile(std::shared_ptr<Type> type);
std::shared_ptr<QualifierType> make_const_volatile(std::shared_ptr<Type> type);
std::shared_ptr<Type> remove_qualifier(std::shared_ptr<QualifierType> type);

_CML_END_
#endif // _ZCC_QUALIFIER_TYPE_H_