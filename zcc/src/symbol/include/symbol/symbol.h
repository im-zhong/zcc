
/*
        zhong, ysu
        3.3
        TODO symbol已经基本实现 再就是make_symbol需要视情况做一些补充
*/

#ifndef _ZCC_SYMBOL_H_
#define _ZCC_SYMBOL_H_

/*
        简单的symbol
*/
/*
        zhong, ysu
        3.2
*/

/*
        update 3.5 v1.1
        在Symbol中添加一个qualifier:int 代替Type中的qualifier

*/

#include "core/core.h"
#include <memory>
#include <string>
_CML_BEGIN_

// 前置声明
// 符号类型
class Type;

struct Coordinate {
    std::string filename{};
    int line{};
};

// 定义这俩玩意有什么用??
// SCOPE不能是枚举 因为scope的值可以不断延申
enum class SCOPE : int { CONSTANT = 1, LABELS, GLOBAL, PARAMETER, LOCAL };

enum class STORAGE_CATEGORY : int {
    AUTO,
    REGISTER,
    STATIC,
    EXTERN,
    TYPEDEF,
    ENUM
};

// 因为Symbol的类成员经常需要使用和修改 所以写成public比较方便
struct Symbol {
    std::string name{}; // 符号名
    int scope{};
    std::weak_ptr<Type> type{};
    Coordinate coordinate{};             // 符号坐标
    STORAGE_CATEGORY storage_category{}; // sclass 扩展存储类型
    // 3.5 v1.1
    // 3.25 取消 qualifier
    // int qualifier{};

    // 要不让make_symbol 来承担构造的复杂性好了
    // 这里就定义一个完整的需要所有参数的构造函数
    // 然后make_symbol提供不同的构造方式
    Symbol(const std::string& name, int scope, std::shared_ptr<Type> type,
           const std::string& filename, int line,
           STORAGE_CATEGORY storage_category)
        : name(name), scope(scope), coordinate{filename, line},
          storage_category(storage_category), type(type) {}

    Symbol() = default;
    Symbol(const Symbol&) = default;
    Symbol& operator=(const Symbol&) = default;
    Symbol(Symbol&&) = default;
    Symbol& operator=(Symbol&&) = default;
    ~Symbol() = default;

    // 提供对type的更加方便的访问
    // 如果类型失效就返回nullptr
    std::shared_ptr<Type> get_type() const noexcept { return type.lock(); }

    // 就提供get size he  get align 吧
    int get_size() const noexcept;
    int get_align() const noexcept;

    std::string to_string() const noexcept;
};

// 3.25 symbol是可以携带值的 最起码constsymbol就需要携带值 不然无法进行计算
template <typename ValueType> class ConstantSymbol : public Symbol {
  public:
    ValueType value;
};

// 提供静态函数 用来生成shared_ptr
// 用这些函数作为工厂函数
// 提供一个默认构造的符号
std::shared_ptr<Symbol> make_symbol();

std::shared_ptr<Symbol>
make_symbol(const std::string& name, int scope, std::shared_ptr<Type> type,
            Coordinate coordinate = {},
            STORAGE_CATEGORY storage_category = STORAGE_CATEGORY::AUTO);
_CML_END_
#endif // _ZCC_SYMBOL_H_