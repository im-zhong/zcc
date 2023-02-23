#pragma once

#ifndef _ZCC_TYPE_TABLE_H_
#define _ZCC_TYPE_TABLE_H_

#include "type.h"
#include "util/scoped_table.h"

_CML_BEGIN_
// 如何保存一个类型呢 ??
// 通常我们可以通过一个 string 来找到一个类型
// typedef 就是这样做的
// 同样 struct union enum 都是这样做的
// 我忘了我实现的符号表就是以string作为key的、
// 我只需要指定valuetype = std::shared_ptr<Type>

// 或许我应该用一个类把这些东西包起来
using TypeTable = ScopedTable<std::shared_ptr<Type>>;

TypeTable global_type_table;
TypeTable extern_type_talbe;

_CML_END_
#endif // _ZCC_TYPE_TABLE_H_