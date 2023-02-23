#pragma once
/*
        3.6
        定义符号表接口
*/

#ifndef _CML_TABLE_INTERFACE_H_
#define _CML_TABLE_INTERFACE_H_

#include <string>

template <typename ValueType> class TableInterface {
  public:
    using key_type = std::string;
    using value_type = ValueType;

    virtual value_type find(const key_type& name) const = 0;
    virtual bool insert(const key_type& name, const value_type& symbol) = 0;
};

#endif // _CML_TABLE_INTERFACE_H_