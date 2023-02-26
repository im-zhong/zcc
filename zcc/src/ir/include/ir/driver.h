// 2023/2/25
// zhangzhong
// ir driver
#pragma once

#include "parser.hpp"
#include <string>

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

  private:
    std::string filename;
    yy::location location;
};
} // namespace ir