// 2023/2/25
// zhangzhong
// ir driver
#pragma once

#include "ir.h"
#include "parser.hpp"
#include <string>
#include <variant>
#include <vector>

namespace ir {
class driver;
}

// modify flex yylex signature
#define YY_DECL yy::parser::symbol_type yylex(ir::driver& drv)
// declare yylex
YY_DECL;

namespace ir {
class driver {
  public:
    driver() = default;

    // handle the scanner
    void begin_scan();
    void end_scan();

    // start parse
    int parse(const std::string& filename);

    yy::location& get_location();
    CodeList& get_code_list();

    void dump() const;

  private:
    std::string filename;
    yy::location location;

    // 现在也不能用这个了 必须使用variant
    CodeList code_list;
};
} // namespace ir