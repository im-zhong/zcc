/*
        3.6 符号表基础功能实现
*/

#include "symbol/scoped_table.h"
#include "symbol/unscoped_table.h"
#include <iostream>

using namespace std;

void Test_unorder_map() {
    cout.setf(ios::boolalpha);
    std::unordered_map<std::string, int> table;
    auto ri = table.insert({"1", 1});
    ri.first;  // 插入成功返回插入元素的迭代器
    ri.second; // bool 代表插入是否成功
    cout << ri.second << endl;
    ri = table.insert({"1", 2});
    cout << ri.second << endl;

    auto rf = table.find("1");
    // rf 是一个迭代器
    if (rf != table.end()) {
        cout << "查找成功" << endl;
        cout << rf->first;
        cout << rf->second;
    } else {
        cout << "查找失败" << endl;
    }

    auto re = table.erase("1");
    rf = table.find("1");
}

shared_ptr<int> make_int(int i) {
    return std::make_shared<int>(i);
    // return std::shared_ptr<int>(new int(i));
}

void Test_unscoped_table() {
    IntTable table;
    auto rr = table.insert("1", 1);
    auto rf = table.find("1");
}

void Test_scoped_table() {
    ScopedTable<shared_ptr<int>> table;
    cout << table.to_string() << endl;
    table.insert("1", make_int(1));
    table.find("1");
    cout << table.to_string() << endl;
    table.enter_scope();
    table.enter_scope();
    cout << table.to_string();
    table.leave_scope();
    table.insert("1", make_int(1));
    table.insert("2", make_int(2));
    table.find("1");
    cout << table.to_string() << endl;
    table.leave_scope();
    table.enter_scope();
    cout << table.to_string() << endl;
    // 但是不分配符号
    table.leave_scope();
    cout << table.to_string() << endl;
}

// 将 scopetable 和 symbol 做联合测试

int main() {
    // Test_unorder_map();
    // Test_unscoped_table();
    Test_scoped_table();
}