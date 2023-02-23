/*
        这个文件就是思考了一下中间代码如何生成
        只要我们可以再第一遍遍历的时候生成一个语法树
        那么我们就可以从这棵树上 生成相应的中间代码
        只要有结构就行 书上一遍生成的复杂度太高了 难以理解
        分两趟就简单多了
*/
#ifndef _ZCC_SUFFIX_STACK_H_
#define _ZCC_SUFFIX_STACK_H_

#include "core/core.h"
#include <stack>

_CML_BEGIN_

class Operator {
  public:
    // 这个函数工作需要一个参数 那就是suffix stack
    virtual void do_operator();
};

template <typename Token> class SuffixStack {
  public:
    using value_type = Token;

    // 对于每一个特定的token 我们应该可以判断他是否位operator
    // 如果是operator 那么他应该可以执行相应的操作
    // 我们应该可以从这个operator token 获取相应的对象 并给予token stack 然后
    // 它可以完成相应的操作

  private:
    std::stack<value_type> sufix_stack;
};

_CML_END_

#endif // _ZCC_SUFFIX_STACK_H_