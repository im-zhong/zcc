// 2023/2/25
// zhangzhong
// ir driver

#include "ir/driver.h"
#include "ir/location.hh"
#include "ir/parser.hpp"
#include <iostream>
#include <variant>

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

DeclList& driver::get_decl_list() { return this->decl_list; }

void driver::dump() const {
    for (const auto& decl : this->decl_list) {
        // visit就是遍历variant的值
        // 这里的auto推导的类型是 shared_ptr
        // 然后to_string是一个模板函数 会调用类型的to_string
        std::visit(
            [](auto decl) { std::cout << to_string(*decl) << std::endl; },
            decl);
    }
}

} // namespace ir
