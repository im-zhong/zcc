#pragma once

#include "symbol.h"
#include "util/table.h"

using SymbolTable = ScopedTable<std::shared_ptr<Symbol>>;