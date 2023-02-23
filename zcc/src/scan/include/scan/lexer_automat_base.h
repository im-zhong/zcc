
#ifndef _ZCC_LEXER_AUTOMAT_BASE_H_
#define _ZCC_LEXER_AUTOMAT_BASE_H_

#include "lexer_buffer_adapter.h"
#include <memory>

_CML_BEGIN_

class LexerAutomatBase {
  public:
    LexerAutomatBase(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : buffer(buffer_ptr) {}
    virtual void run() {}

  private:
    virtual void dispatch_init() {}
    virtual void dispatch(uint8_t __category) {}

  protected:
    std::shared_ptr<LexerBufferAdapter> buffer;
};

_CML_END_

#endif // _ZCC_LEXER_AUTOMAT_BASE_H_