// 2023/2/25
// zhangzhong
// ir driver

#include "ir/driver.h"
#include "ir/location.hh"
#include "ir/parser.hpp"

namespace ir {

int driver::parse(const std::string& filename) {
    this->filename = filename;
    this->location.initialize(&this->filename);

    // begin scan
    begin_scan();
    int result = yy::parser{*this}();
    end_scan();
    return result;
}

yy::location& driver::get_location() { return this->location; }

} // namespace ir
