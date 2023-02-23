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

// --> @ rules tip
//  自动机读取规则 ：
//		如果该自动机在peek处结束，那么需要forward为下一个自动的处理做准备
//		如果没有在peek处结束，而是调用了下一个自动机，那么就不需要forward，为下一个自动机保留灵活性

// --> @ todo
//  Escape :: get_escape_8d() ; get_escape_x() undone
//  identifer关于数字起头的id的错误检测这个应该在digit中检测吧
//  简单实现预处理命令 让他尽可能去读取一些信息
//  其实在实现地时候，基本上没有考虑后跟字符是否会产生错误，下一步就是为了完善这些信息
//  当一些子自动机相当大地时候，就应该分离文件，重新组织
//  底层buffer存在一个bug 导致字符数量很大时无法处理
//  grammar_abstract.h 文件中需要填写一个 reserved_map 数据

#ifndef _ZCC_LEXER_H_
#define _ZCC_LEXER_H_

#include "buffer.h"
// #include"buffer.h"
#include "convert.h"
#include "core/grammar_abstract.h"
#include "error.h"
#include "lexer_automat_impl.h"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

_CML_BEGIN_ // namespace cml begin

    // 使用当前peek调度一次自动机
    class LexerAutomat : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;

    virtual void run() override {
        // 通过查看第一个字符进行分类 从而调度不同的子自动机
        // cout << "call LexerAutomat.run start" << endl;
        // get_peek 这个动作就是向前看一眼
        // 但是实际上它查看的是forward以后的
        // 在启动一次新的token获取时 peek = current
        // 在token连续读取过程中 peek = forward ?
        // 最终 begin = end = current = peek = forward
        dispatch(buffer->get_peek());
        // cout << "call LexerAutomat.run end" << endl;
    }

  private:
    virtual void dispatch(uint8_t __category) override {
        // dispatch_init();
        automat_convert[get_automat_category(__category)]->run();
    }

    virtual void dispatch_init() override { buffer->dispatch_init(); }

  protected:
    std::shared_ptr<WhiteSpace> white_space =
        std::make_shared<WhiteSpace>(buffer);
    std::shared_ptr<Identifier> identifier =
        std::make_shared<Identifier>(buffer);
    std::shared_ptr<LiteralInteger> literal_integer =
        std::make_shared<LiteralInteger>(buffer);
    std::shared_ptr<Operator> _operator =
        std::make_shared<Operator>(buffer, literal_integer);
    std::shared_ptr<Delimiter> delimiter = std::make_shared<Delimiter>(buffer);
    std::shared_ptr<LiteralChar> literal_char =
        std::make_shared<LiteralChar>(buffer);
    std::shared_ptr<LiteralString> literal_string =
        std::make_shared<LiteralString>(buffer, literal_char->get_escape());
    std::shared_ptr<PreProcessor> pre_processor =
        std::make_shared<PreProcessor>(buffer);
    std::shared_ptr<BackSlash> back_slash = std::make_shared<BackSlash>(buffer);
    std::shared_ptr<Undefined> undefined = std::make_shared<Undefined>(buffer);

    std::vector<std::shared_ptr<LexerAutomatBase>> automat_convert = {
        white_space,  identifier,     literal_integer, _operator,  delimiter,
        literal_char, literal_string, pre_processor,   back_slash, undefined,
    };
};

// 使用lexer_automat 完成词法分析
class Lexer {
  public:
    Lexer()
        : buffer(std::make_shared<LexerBufferAdapter>()),
          lexer_automat(std::make_shared<LexerAutomat>(buffer)) {}

  public:
    bool load(const std::string& __filename) {
        return buffer->load(__filename);
    }

    // 由于代码改动 函数的意义发生了改变
    // bool is_eof() const { return buffer->is_eof(); }
    bool is_eof() const noexcept { return !buffer->is_forwardable(); }

    int current_line() const { return buffer->get_line(); }
    int lexeme_count() const { return buffer->get_lexeme_count(); }

    Token lexer() {
        while (true) {
            if (is_eof())
                return {static_cast<uint8_t>(eof), ""};
            else {
                lexer_automat->run();
                if (buffer->is_get_lexeme())
                    return buffer->get_token();
                else
                    continue;
            }
        }
    }

  public:
    std::shared_ptr<LexerBufferAdapter> buffer;
    std::shared_ptr<LexerAutomat> lexer_automat;
};

_CML_END_ // namespace cml end

#endif // _ZCC_LEXER_H_