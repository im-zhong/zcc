// 2023/2/25
// zhangzhong
// ir type

namespace ir {
// ir 有自己的类型系统
class Type {};
class BasicType : public Type {};
class StructType : public Type {};
class FnType : public Type {};
class PointerType : public Type {};

} // namespace ir