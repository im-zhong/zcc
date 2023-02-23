#pragma once
/*
        临时变量

        这TM不就是定义一个Symbol吗 ???
*/

#include "symbol.h"

// 临时生成的局部变量
// 产生一些局部变量用于函数传递和返回结构
// 用来保存条件表达式的结果和switch语句的测试值
class LocalSymbol : public Symbol {};

// 临时变量
class TemporarySymbol : public Symbol {
  public:
    bool temoporary;
};

// 仅仅产生一个Symbol而已
Symbol make_temp(Type type, int storage_category, int scope);
