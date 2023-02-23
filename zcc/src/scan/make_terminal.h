//
// Copyright (c) 2019.03
// Zhong
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.
//

// --> @ think
//  token, terminal 的功能很像  terminal必须是模板的以容纳不同的terminal类型
//  现在的问题是 Terminal是否应该继承token
//	 非终结符属性用 category 还是 tag 如果为了一致性的话 tag应该更名为
// category
//

#ifndef _ZCC_MAKE_TERMINAL_H_
#define _ZCC_MAKE_TERMINAL_H_

#include "lexer.h"
#include <stdint.h>

_CML_BEGIN_ // namespace cml begin

    struct Terminal_Base {
    uint8_t category;
};

template <typename Ty> struct Terminal : public Terminal_Base {
    Terminal(uint8_t __tag, const Ty& __value = Ty())
        : Terminal_Base{__tag}, value(__value) {}
    Ty value = 0;
};

template <> struct Terminal<std::string> : public Terminal_Base {

    Terminal() = default;

    Terminal(const Token& __token)
        : Terminal_Base{static_cast<uint8_t>(__token.category)},
          value(__token.lexeme) {}

    Terminal(uint8_t __tag, const std::string& __value = std::string())
        : Terminal_Base{__tag}, value(__value) {}
    std::string value;
};

// 对于 lexeme 就是 value 的token来货 直接调用这个函数即可
inline std::shared_ptr<Terminal<std::string>>
make_terminal_lexeme(const Token& token) {
    return std::make_shared<Terminal<std::string>>(token);
}

inline std::shared_ptr<Terminal<int>> make_terminal_int(const Token& token) {
    std::shared_ptr<Terminal<int>> terminal_integer =
        std::make_shared<Terminal<int>>(token.category);
    terminal_integer->value = std::stoi(token.lexeme);
    return terminal_integer;
}

inline std::shared_ptr<Terminal<double>>
make_terminal_real(const Token& token) {
    std::shared_ptr<Terminal<double>> terminal_real =
        std::make_shared<Terminal<double>>(token.category);
    terminal_real->value = std::stod(token.lexeme);
    return terminal_real;
}

inline std::shared_ptr<Terminal<char>> make_terminal_char(const Token& token) {
    std::shared_ptr<Terminal<char>> terminal_char =
        std::make_shared<Terminal<char>>(token.category, 0);
    terminal_char->value = token.lexeme.front();
    return terminal_char;
}

inline std::shared_ptr<Terminal<std::string>>
make_terminal_id(const Token& token) {
    auto id_categoty = check_reserved(token.lexeme);
    if (id_categoty == static_cast<uint8_t>(-1)) // id
        return std::make_shared<Terminal<std::string>>(token.category,
                                                       token.lexeme);
    else // reserved word
        return std::make_shared<Terminal<std::string>>(id_categoty,
                                                       token.lexeme);
}

std::shared_ptr<Terminal_Base> make_terminal(const Token& token) {
    switch (token.category) {
    case TERMINAL::ID:
        return make_terminal_id(token);
    case TERMINAL::LIT_INT:
        return make_terminal_int(token);
    case TERMINAL::LIT_REAL:
        return make_terminal_real(token);
    case TERMINAL::LIT_CHAR:
        return make_terminal_char(token);
    default:
        return make_terminal_lexeme(token);
    }
}

_CML_END_ // namespace cml end

#endif // _ZCC_MAKE_TERMINAL_H_
