
#include "type/qualifier_type.h"
_CML_BEGIN_
std::shared_ptr<QualifierType> make_const(std::shared_ptr<Type> type) {
    return std::make_shared<QualifierType>(type, const_mask);
}

std::shared_ptr<QualifierType> make_volatile(std::shared_ptr<Type> type) {
    return std::make_shared<QualifierType>(type, volatile_mask);
}

std::shared_ptr<QualifierType> make_const_volatile(std::shared_ptr<Type> type) {
    return std::make_shared<QualifierType>(type, const_mask | volatile_mask);
}

std::shared_ptr<Type> remove_qualifier(std::shared_ptr<QualifierType> type) {
    return type->get_inner_type();
}

std::string QualifierType::to_string() const noexcept {
    std::string temp;
    if (is_const(this->type_word)) {
        temp += "const ";
    }
    if (is_volatile(this->type_word)) {
        temp += "volatile ";
    }
    temp += inner_type->to_string();
    return temp;
}
_CML_END_