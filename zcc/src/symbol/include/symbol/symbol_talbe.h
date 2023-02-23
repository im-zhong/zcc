#pragma once

#include "symbol.h"
#include "table.h"

using SymbolTable = ScopedTable<std::shared_ptr<Symbol>>;