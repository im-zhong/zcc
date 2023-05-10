// 2023/5/10
// zhangzhong
// string table

#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

namespace util {
// string view非常合适用来实现string table
class StringTable {
  public:
    auto insert(std::string str) -> std::string_view {
        auto [it, _] = table.insert(std::move(str));
        return *it;
    }

    // auto insert(std::string_view str) -> std::string_view {
    //     auto [it, _] = table.insert(std::string(str));
    //     return *it;
    // }

    auto contains(std::string_view str) -> bool {
        return table.find(std::string(str)) != table.end();
    }

  private:
    std::unordered_set<std::string> table;
};

} // namespace util
