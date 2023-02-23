#pragma once

#ifndef _ZCC_SYNTAX_ERROR_H_
#define _ZCC_SYNTAX_ERROR_H_

#include "core/core.h"
#include <iostream>
#include <string>

_CML_BEGIN_

static void syntax_error(const std::string& message) {
    std::cerr << message << std::endl;
}

_CML_END_

#endif // _ZCC_SYNTAX_ERROR_H_