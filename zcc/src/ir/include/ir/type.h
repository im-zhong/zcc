// 2023/2/25
// zhangzhong
// ir type

#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ir {

// shared_ptr<Type> 作为类型是非常非常合适的
// 1. 它保证了类型只有一个副本
// 2. 类型之间的比较也非常方便 直接使用 shared_ptr 的 ==
// 因为只有一个副本吗
struct BasicType;
struct StructType;
struct FnType;
struct PointerType;
using BasicTypePtr = std::shared_ptr<BasicType>;
using StructTypePtr = std::shared_ptr<StructType>;
using FnTypePtr = std::shared_ptr<FnType>;
using PointerTypePtr = std::shared_ptr<PointerType>;
using Type =
    std::variant<BasicTypePtr, StructTypePtr, FnTypePtr, PointerTypePtr>;
using TypeList = std::vector<Type>;
TypeList make_empty_type_list();

// 类型想要进行比较 必须提供一个type table
using TypeTable = std::unordered_map<std::string, Type>;

// ir 有自己的类型系统
struct BasicType {
    int type;

    std::string to_string() const;
};
BasicTypePtr make_basic_type(int type);

// todo
// 这个东西需要保存类型
struct StructType {
    // type list
    std::string name;

    std::string to_string() const;
};
StructTypePtr make_struct_type(std::string name);

struct FnType {
    TypeList parameter_type;
    std::optional<Type> return_type;

    std::string to_string() const;
};
FnTypePtr make_fn_type(TypeList parameter_type,
                       std::optional<Type> return_type = std::nullopt);

struct PointerType {
    Type type;

    std::string to_string() const;
};
PointerTypePtr make_pointer_type(Type type);

} // namespace ir
