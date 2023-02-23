#pragma once

#ifndef _ZCC_TOKEN_H_
#define _ZCC_TOKEN_H_

#include "core/core.h"
#include <iostream>

_CML_BEGIN_

// TODO token需要附加一个coordinate功能 这样我们可以定位词素的位置
// 也就可以定位错误
struct Token {
    uint8_t category = -1; // tag
    std::string lexeme;    // 词素

    bool operator==(const Token& rhs) const noexcept {
        return this->category == rhs.category && this->lexeme == rhs.lexeme;
    }
};

// Token token;

Token make_token(uint8_t category, const std::string& lexeme);

std::ostream& operator<<(std::ostream& os, const Token& token);

_CML_END_

#endif // _ZCC_TOKEN_H_