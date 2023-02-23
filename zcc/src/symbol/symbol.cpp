// //#include "symbol.h"
// #include <zcc/type/type.h>
// _CML_BEGIN_

// std::string Symbol::to_string() const noexcept
// {
// 	std::string symbol_string;

// 	symbol_string += name;

// 	auto shared_type = type.lock();
// 	if (shared_type)
// 	{
// 		symbol_string += shared_type->to_string();
// 	}

// 	symbol_string += std::to_string(scope);
// 	symbol_string += coordinate.filename;
// 	symbol_string += coordinate.line;
// 	symbol_string += std::to_string(enum_cast(storage_category));

// 	return symbol_string;
// }

// int Symbol::get_size() const noexcept
// {
// 	auto symbol_type = get_type();
// 	if (symbol_type)
// 		return symbol_type->size;
// 	else
// 		return 0;
// }

// int Symbol::get_align() const noexcept
// {
// 	auto symbol_type = get_type();
// 	if (symbol_type)
// 		return symbol_type->align;
// 	else
// 		return 0;
// }

// // 提供静态函数 用来生成shared_ptr
// // 用这些函数作为工厂函数
// // 提供一个默认构造的符号
// std::shared_ptr<Symbol> make_symbol()
// {
// 	return std::make_shared<Symbol>();
// }

// std::shared_ptr<Symbol> make_symbol(const std::string& name,
// 	int scope, std::shared_ptr<Type> type,
// 	Coordinate coordinate,
// 	STORAGE_CATEGORY storage_category)
// {
// 	return std::make_shared<Symbol>(name, scope, type, coordinate.filename,
// coordinate.line, storage_category);
// }

// _CML_END_