
#include "core/core.h"
#include "type/type.h"

_CML_BEGIN_
std::string Type::to_string() const noexcept {
    // native type to string
    auto type = get_type();
    std::string type_string;
    // 这里似乎只能使用switch
    switch (type) {
    case enum_cast(TYPE::VOID):
        type_string = typeid(void).name();
        break;
    case enum_cast(TYPE::CHAR):
        type_string = typeid(char).name();
        break;
    case enum_cast(TYPE::SHORT):
        type_string = typeid(short).name();
        break;
    case enum_cast(TYPE::INT):
        type_string = typeid(int).name();
        break;
    case enum_cast(TYPE::LONG):
        type_string = typeid(long).name();
        break;
    case enum_cast(TYPE::FLOAT):
        type_string = typeid(float).name();
        break;
    case enum_cast(TYPE::DOUBLE):
        type_string = typeid(double).name();
        break;
    case enum_cast(TYPE::VARIABLE):
        type_string = "...";
        break;
    }
    return type_string;
}

std::string ArrayType::to_string() const noexcept {
    return "array to " + get_element_type()->to_string() + " of " +
           std::to_string(element_size);
}

std::string PointerType::to_string() const noexcept {
    return "pointer to " + dereference_type()->to_string();
}

std::string FunctionType::to_string() const noexcept {
    std::string function_type_string = return_type->to_string();
    function_type_string += "(";
    if (!parameter_list_type.empty()) {
        for (const auto& parameter : parameter_list_type) {
            function_type_string += parameter->to_string();
            function_type_string += ", ";
        }
        // 删掉最后面的一个空格和逗号
        function_type_string.pop_back();
        function_type_string.pop_back();
    }
    // 在最后添上
    function_type_string.push_back(')');
    return function_type_string;
}

std::string EnumType::to_string() const noexcept { return "enum " + name; }

std::string UnionType::to_string() const noexcept { return "union " + name; }

std::string StructType::to_string() const noexcept { return "struct " + name; }
_CML_END_