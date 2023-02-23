#include "token.h"
#include <iostream>

using namespace std;

_CML_BEGIN_

Token make_token(uint8_t category, const std::string& lexeme) {
    return {category, lexeme};
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "(" << static_cast<int>(token.category) << ", " << token.lexeme
       << ")";
    return os;
}

_CML_END_
