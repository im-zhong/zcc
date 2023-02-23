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

#ifndef _ZCC_ERROR_H_
#define _ZCC_ERROR_H_

#include "core/core.h"
#include <iostream>
#include <string>

_CML_BEGIN_ // namespace cml begin

    class My_Cml_Error {
  public:
    virtual void operator()(const std::string& __error_message) {
        std::cerr << __error_message << std::endl;
        exit(EXIT_FAILURE);
    }
};

class lexer_error : public My_Cml_Error {};

_CML_END_ // namespace cml end

#endif // _ZCC_ERROR_H_
