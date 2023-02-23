#pragma once

#ifndef _ZCC_TYPE_CHECK_H_
#define _ZCC_TYPE_CHECK_H_

#include "core/core.h"
#include <memory>

_CML_BEGIN_
class Type;

// 检查两个类型是否相等
bool is_same(const std::shared_ptr<Type>& lhs,
             const std::shared_ptr<Type>& rhs) noexcept;

// 检查两个类型是否兼容
bool type_check(const std::shared_ptr<Type>& lhs,
                const std::shared_ptr<Type> rhs) noexcept;

// 好像还可以整型提升
// 小整型提升到整型或无符号数
// float提升到double
// 如果不是这些类型返回原类型
std::shared_ptr<Type> type_promote(const std::shared_ptr<Type>& type) noexcept;
_CML_END_
#endif // _ZCC_TYPE_CHECK_H_