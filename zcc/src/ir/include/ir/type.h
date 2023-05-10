// 2023/2/25
// zhangzhong
// ir type

#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <string>
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

// 想给variant加方法 只能用模板的方式来加

// 每一个函数都需要一个operator overloader
// 同时也需要一个concept
template <typename T>
concept typable = requires(T t) {
    { t.to_string() } -> std::same_as<std::string>;
    { t.is_function() } -> std::same_as<bool>;
    { t.get_size() } -> std::same_as<int>;
};

// 然后给每个函数添加一个overloader

// ir 有自己的类型系统
struct BasicType {
    int type;

    std::string to_string() const;
    bool is_function() const;
    int get_size() const;
};
BasicTypePtr make_basic_type(int type);

// todo
// 这个东西需要保存类型
struct StructType {
    // type list
    std::string name;

    std::string to_string() const;
    bool is_function() const;
    int get_size() const;
};
StructTypePtr make_struct_type(std::string name);

struct FnType {
    TypeList parameter_type;
    bool varargs;
    std::optional<Type> return_type;
    std::string to_string() const;
    bool is_function() const;
    int get_size() const;
};
FnTypePtr make_fn_type(FnType fn_type);

struct PointerType {
    Type type;

    std::string to_string() const;
    bool is_function() const;
    int get_size() const;
};
PointerTypePtr make_pointer_type(Type type);

// template <typable T> struct call_to_string {
//     std::string operator()(std::shared_ptr<T> type) {
//         return type->to_string();
//     }
// };

// 这些类型还需要计算大小 type.get_size();
// type.is_function() 不过分吧

struct call_is_function {
    void operator()(const BasicTypePtr& type) { result = type->is_function(); }
    void operator()(const StructTypePtr& type) { result = type->is_function(); }
    void operator()(const FnTypePtr& type) { result = type->is_function(); }
    void operator()(const PointerTypePtr& type) {
        result = type->is_function();
    }
    bool& result;
};

struct call_get_size {
    void operator()(const BasicTypePtr& type) { size = type->get_size(); }
    void operator()(const StructTypePtr& type) { size = type->get_size(); }
    void operator()(const FnTypePtr& type) { size = type->get_size(); }
    void operator()(const PointerTypePtr& type) { size = type->get_size(); }
    int& size;
};

} // namespace ir
