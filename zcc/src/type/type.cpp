
#include "type/type.h"
_CML_BEGIN_
// -- > pointer

std::shared_ptr<PointerType> make_pointer(const std::shared_ptr<Type>& type) {
    return std::make_shared<PointerType>(type);
}

// 对一个指针类型进行解引用
// std::shared_ptr<Type> make_dereference(std::shared_ptr<Type> pointer_type)
//{
//	// 首先检查是不是指针类型
//	if (pointer_type->get_operator() == enum_cast(TYPE_OPERATOR::POINTER))
//		return pointer_type->get_inner_type();
//	else
//		return nullptr;
//}

// --> array

std::shared_ptr<ArrayType> make_array(const std::shared_ptr<Type>& type,
                                      size_t element_count) {
    return std::make_shared<ArrayType>(type, element_count);
}

// std::shared_ptr<PointerType> make_array_decay(std::shared_ptr<Type>
// array_type)
//{
//	// 首先检查是不是array
//	if (array_type->get_operator() == enum_cast(TYPE_OPERATOR::ARRAY))
//	{
//		return make_pointer(array_type->get_inner_type());
//	}
//	else
//		return nullptr;
// }

// --> function

std::shared_ptr<FunctionType>
make_function(const std::shared_ptr<Type>& return_type,
              const std::list<std::shared_ptr<Type>>& parameter_list_type) {
    return std::make_shared<FunctionType>(return_type, parameter_list_type);
}

// std::shared_ptr<PointerType>
// make_function_decay(std::shared_ptr<FunctionType> function_type)
//{
//	return make_pointer(function_type);
// }

// --> enum

std::shared_ptr<EnumType>
make_enum(const std::string& enum_name,
          const std::list<std::shared_ptr<Symbol>>& enum_constants) {
    return std::make_shared<EnumType>(enum_name, enum_constants);
}

// --> struct

std::shared_ptr<Field> make_field(const std::shared_ptr<Symbol>& symbol,
                                  int offset) {
    return std::make_shared<Field>(symbol, offset);
}

// std::shared_ptr<StructField> make_struct_field()
//{
//	return std::make_shared<StructField>();
// }

// std::shared_ptr<StructType> make_struct(const std::string& struct_name,
//	std::shared_ptr<StructField> field, bool defined)
//{
//	return std::make_shared<StructType>(struct_name, field, defined);
// }

std::shared_ptr<StructType>
make_struct(const std::string& struct_name,
            const std::list<std::shared_ptr<Symbol>>& field_symbols,
            bool is_define) {
    return std::make_shared<StructType>(struct_name, field_symbols, is_define);
}

std::shared_ptr<StructType> make_struct(const std::string& struct_name,
                                        bool is_define) {
    return std::make_shared<StructType>(struct_name, is_define);
}

std::shared_ptr<StructType>
make_struct(const std::list<std::shared_ptr<Symbol>>& field_symbols) {
    auto struct_name = make_random_name();
    return make_struct(struct_name, field_symbols, true);
}
std::shared_ptr<StructType> make_struct() {
    auto struct_name = make_random_name();
    return make_struct(struct_name, true);
}

// --> union

std::shared_ptr<UnionType> make_union(const std::string& union_name,
                                      bool is_define) {
    return std::make_shared<UnionType>(union_name, is_define);
}

std::shared_ptr<UnionType>
make_union(const std::string& union_name,
           const std::list<std::shared_ptr<Symbol>>& field_symbols,
           bool is_define) {
    return std::make_shared<UnionType>(union_name, field_symbols, is_define);
}

std::shared_ptr<UnionType> make_union() {
    auto union_name = make_random_name();
    return std::make_shared<UnionType>(union_name, true);
}

std::shared_ptr<UnionType>
make_union(const std::list<std::shared_ptr<Symbol>>& field_symbols) {
    auto union_name = make_random_name();
    return std::make_shared<UnionType>(union_name, field_symbols, true);
}

// --> depareted

// 这些函数的实现过程都是一样的
// std::shared_ptr<Type> make_const(std::shared_ptr<Type> type)
//{
//	return type->make_const();
//}
//
// std::shared_ptr<Type> make_volatile(std::shared_ptr<Type>& type)
//{
//	return nullptr;
//}
//
// std::shared_ptr<Type> make_const_volatile(std::shared_ptr<Type>& type)
//{
//	return nullptr;
//}
//
// std::shared_ptr<Type> make_qualifier(std::shared_ptr<Type>& type)
//{
//	return nullptr;
//}
//
// std::shared_ptr<Type> remove_qualifier(std::shared_ptr<Type>& type)
//{
//	return nullptr;
//}

_CML_END_