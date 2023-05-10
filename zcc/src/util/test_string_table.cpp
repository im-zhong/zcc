// 2023/5/10
// zhangzhong

#include "util/string_table.h"
#include <cassert>

int main() {
    util::StringTable table;
    auto a = table.insert("hello");
    auto b = table.insert("world");
    auto c = table.insert("hello");
    assert(a == c);
    assert(a != b);
    assert(table.contains("hello"));
    assert(table.contains("world"));
    assert(!table.contains("zhangzhong"));
}
