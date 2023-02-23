
#ifndef _ZCC_LEXER_BUFFER_ADAPTER_H_
#define _ZCC_LEXER_BUFFER_ADAPTER_H_

#include "buffer.h"
#include "token.h"
#include <string>

_CML_BEGIN_

using namespace cml;

class LexerBufferAdapter {
  public:
    LexerBufferAdapter() = default;

    LexerBufferAdapter(const std::string& __filename) : buffer(__filename) {
        peek = buffer.current();
    }

  public:
    bool is_forwardable() const noexcept { return buffer.is_forwardable(); }
    bool load(const std::string& __filename) {
        buffer.load(__filename);
        peek = buffer.current();
        return true;
    }
    void forward() { peek = buffer.forward(); }
    void skip() {
        buffer.skip_lexeme();
        done = false;
    }
    void inc_line() { ++line; }
    int get_line() const { return line; }
    int get_lexeme_count() const { return lexeme_count; }
    bool is_get_lexeme() const { return done; }
    bool is_eof() const { return buffer.is_eof(); }

    // look_lexeme必须实现
    std::string look_lexeme() const { return buffer.look_lexeme(); }
    // std::string look_lexeme() const { return "look_lexeme_test"; }
    std::string get_lexeme() const { return token.lexeme; }
    uint8_t get_category() const { return token.category; }
    Token get_token() const { return token; }

    const char& get_peek() const { return peek; }
    char& get_peek() { return peek; }
    uint8_t get_last_category() const { return last_category; }

    bool is_peek(char c) const { return peek == c; }
    bool is_peek(std::initializer_list<char> il) const {
        for (const char c : il)
            if (peek == c)
                return true;
        return false;
    }
    bool is_forward(char c) {
        forward();
        return peek == c;
    }
    bool is_forward(std::initializer_list<char> il) {
        forward();
        return is_peek(il);
    }

    void set_token(uint8_t __category) {
        set_token(__category, buffer.get_lexeme());
    }

    void set_token(uint8_t __category, const std::string& __lexeme) {
        last_category = token.category;
        token.category = __category;
        token.lexeme = __lexeme;
        ++lexeme_count;
        done = true;
    }

    // 这个函数真的啥都没干 其实没必要带着
    void dispatch_init() // 这个函数应该没用
    {
        done = false;
    }

  protected:
    Buffer buffer;
    char peek = Buffer::eof;
    int line = 1;
    bool done = false;
    Token token;
    uint8_t last_category = 0;
    int lexeme_count = 0;
}; // class Lexer_Adapter

_CML_END_
#endif // _ZCC_LEXER_BUFFER_ADAPTER_H_