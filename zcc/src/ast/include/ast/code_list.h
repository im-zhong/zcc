
#ifndef _ZCC_CODE_LIST_H_
#define _ZCC_CODE_LIST_H_

#include "code.h"
#include <list>

_CML_BEGIN_

class CodeList {
  public:
    std::list<Code> code_list;
};

_CML_END_

#endif // _ZCC_CODE_LIST_H_