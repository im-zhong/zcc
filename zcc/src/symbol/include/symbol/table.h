#pragma once
/*
        3.6 table.h
        重定义ScopedTalbe或者UnscopedTable 以适应不同的需求
        注意 table的值类型必须时智能指针
*/

#ifndef _CML_TABLE_H_
#define _CML_TABLE_H_

#include "symbol.h"
#include "util/scoped_table.h"
#include "util/unscoped_table.h"

_CML_BEGIN_
using SymbolTable = ScopedTable<std::shared_ptr<Symbol>>;
_CML_END_
#endif // _CML_TABLE_H_