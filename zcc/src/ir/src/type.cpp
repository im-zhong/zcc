// 2023/2/27
// zhangzhong
// ir type

#include "ir/type.h"
#include "ir/ir.h"
#include <memory>
#include <string>

namespace ir {

TypeList make_empty_type_list() { return {}; }

BasicTypePtr make_basic_type(int type) {
    return std::make_shared<BasicType>(BasicType{.type = type});
}

StructTypePtr make_struct_type(TypeList fields) {
    return std::make_shared<StructType>(
        StructType{.fields = std::move(fields)});
}

FnTypePtr make_fn_type(TypeList parameter_type, Type return_type) {
    return std::make_shared<FnType>(FnType{
        .parameter_type = std::move(parameter_type),
        .return_type = std::move(return_type),
    });
}

PointerTypePtr make_pointer_type(Type type) {
    return std::make_shared<PointerType>(PointerType{
        .type = std::move(type),
    });
}

std::string BasicType::to_string() const {
    switch (type) {
    case IR::BOOL:
        return "bool";
    case IR::I8:
        return "i8";
    case IR::I16:
        return "i16";
    case IR::I32:
        return "i32";
    case IR::I64:
        return "i64";
    case IR::F32:
        return "f32";
    case IR::F64:
        return "f64";
    default:
        exit(EXIT_FAILURE);
    }
}

std::string StructType::to_string() const { return "todo"; }

std::string FnType::to_string() const { return "todo"; }

std::string PointerType::to_string() const {
    std::stringstream ss;
    ss << "*";
    std::visit([&ss](auto type) { ss << ir::to_string(*type); }, type);
    return ss.str();
}

} // namespace ir
