// 2020/2/29
// zhong

/*
        ZCC core.h
        2020/3/8 11:48
        zhong, ysu
*/

/*
        类内排列顺序
        构造
        get/set
        功能性函数 按依赖型/重要性怕 排序
        数据成员
*/

#ifndef _ZCC_CORE_H_
#define _ZCC_CORE_H_

#define _CML_BEGIN_ namespace cml {
#define _CML_END_                                                              \
    }                                                                          \
    ;

#include <string>
#define ERROR_POSITION                                                         \
    (std::string(__FILE__) + " -->> " + std::string(__FUNCTION__) + "\n\t")
#define ERROR_MESSAGE(message) (ERROR_POSITION + (message) + "\n")

#define TEST

// 2020/4/23 定义容器宏
#define Container(container) container.begin(), container.end()

#include <type_traits>
// 枚举值提取工具
_CML_BEGIN_
template <typename Enum> constexpr auto enum_cast(Enum enumeration) noexcept {
    static_assert(std::is_enum_v<Enum>, "Enum must be a enum type!");
    return static_cast<std::underlying_type_t<Enum>>(enumeration);
}
_CML_END_

// file to string
_CML_BEGIN_
std::string file_to_string(const std::string& filename);
_CML_END_

// constant
_CML_BEGIN_

// constexpr char eof = EOF;

_CML_END_

// type
_CML_BEGIN_

// 仅使用大写字母和小写字母就可以了
std::string make_random_string(size_t length);
std::string make_random_name();

_CML_END_

// allocator
_CML_BEGIN_

/*
        zhong, ysu
        20020.3.1
        内存管理模块 初步测试通过！

        警告 所有的注释都不准删！！！

        Tips：在写每个模块的时候，可能都需要一个core文件。那就直接在当前项目创建一个
                  最后将core整合在一起即可 如果core太大也可以分开
   按照模块重新命名 如当前core应该重新命名为 allocator_core.h 即 项目名+core
   全小写
*/

// 返回距离x最近的n的倍数，n是2的幂
constexpr size_t round_up(size_t x, size_t n) noexcept {
    return (x + n - 1) & (~(n - 1));
}

constexpr size_t round_down(size_t x, size_t n) noexcept {
    return x & ~(n - 1);
}

_CML_END_

// lexer
_CML_BEGIN_

/*

        . period 句号
        , comma 逗号
        : colon 冒号
        ; semicolon 分号
        ! exclamation 叹号
        ? question mark 问好
        ' single quotation mark
        " double quotation mark
        () parentheses
        [] square brackets
        ... ellipsis
        / virgule slash divide
        \ back slash
        & ampersand = and
        ~
        % percent mod
        + plus
        - minus
        '* multiplied
        / divide
        * asterisk 星号
        ' apostrophe 撇号
        () brackets 括号
        --- dash 破折号

*/

_CML_END_

// test
_CML_BEGIN_

void print_test_result(bool result);

_CML_END_

#endif // _ZCC_CORE_H_