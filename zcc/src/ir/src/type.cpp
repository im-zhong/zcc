// 2023/2/27
// zhangzhong
// ir type

#include "ir/type.h"
#include "ir/ir.h"
#include <memory>
#include <sstream>
#include <string>
#include <variant>

namespace ir {

TypeList make_empty_type_list() { return {}; }

BasicTypePtr make_basic_type(int type) {
    return std::make_shared<BasicType>(BasicType{.type = type});
}

StructTypePtr make_struct_type(std::string name) {
    return std::make_shared<StructType>(StructType{.name = name});
}

FnTypePtr make_fn_type(FnType fn_type) {
    return std::make_shared<FnType>(fn_type);
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

std::string StructType::to_string() const { return name; }

std::string FnType::to_string() const {
    std::stringstream ss;
    ss << "fn(";
    for (size_t i = 0; i < parameter_type.size(); ++i) {
        ss << type_to_string(parameter_type[i]);
        if (i < parameter_type.size() - 1) {
            ss << ", ";
        }
    }
    if (varargs) {
        ss << ", ...";
    }
    ss << ")";
    if (return_type) {
        ss << "->" << type_to_string(*return_type);
    }
    return ss.str();
}

std::string PointerType::to_string() const {
    std::stringstream ss;
    ss << "*";
    std::visit([&ss](auto type) { ss << ir::to_string(*type); }, type);
    return ss.str();
}

bool BasicType::is_function() const { return false; }
bool StructType::is_function() const { return false; }
bool FnType::is_function() const { return true; }
bool PointerType::is_function() const { return false; }

int BasicType::get_size() const {
    switch (type) {
    // 不对呀 这个I8不就是bool吗
    case IR::BOOL:
        return 1;
    case IR::I8:
        return 1;
    case IR::I16:
        return 2;
    case IR::I32:
        return 4;
    case IR::I64:
        return 8;
    case IR::F32:
        return 4;
    case IR::F64:
        return 8;
    default:
        printf("basic type get size bug\n");
        return -1;
    }
}
int StructType::get_size() const { return 8; }
int FnType::get_size() const { return 8; }
int PointerType::get_size() const { return 8; }

} // namespace ir
