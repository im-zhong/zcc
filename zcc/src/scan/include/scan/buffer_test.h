#pragma once

#ifndef _ZCC_BUFFER_TEST_H_
#define _ZCC_BUFFER_TEST_H_

// 建立一个对象这样比较方便
class BufferTest {
  public:
};

/*
        test

        TODO 撰写测试模块
*/

#include "lexer_buffer_adapter.h"
#include <chrono>
#include <fstream>
#include <random>

using namespace std;
using namespace cml;

#define ERROR_POSITION                                                         \
    (std::string(__FILE__) + " -->> " + std::string(__FUNCDNAME__) + "\n\t")
#define ERROR_MESSAGE(message) (ERROR_POSITION + (message) + "\n")

bool Test_shared_ptr_buffer_adapter(std::shared_ptr<LexerBufferAdapter> buffer,
                                    const std::string& filename) {
    auto file_string = file_to_string(filename);
    uniform_int_distribution<int> u(1, 4095);
    default_random_engine e(
        chrono::steady_clock::now().time_since_epoch().count());

    for (int count = 0; count < 5; ++count) {
        buffer->load(filename);
        string lexeme;
        do {
            for (int i = 0; i < u(e); ++i) {
                buffer->forward();
            }
            buffer->set_token(0);
            auto temp = buffer->get_lexeme();
            lexeme += temp;
        } while (buffer->is_forwardable());

        if (lexeme == file_string)
            ;
        else
            return false;
    }
    for (int count = 0; count < 5; ++count) {
        buffer->load(filename);
        string lexeme;
        do {
            for (int i = 0; i < u(e); ++i) {
                buffer->forward();
            }
            buffer->set_token(0);
            auto temp = buffer->get_lexeme();
            lexeme += temp;
        } while (buffer->is_forwardable());

        if (lexeme == file_string)
            ;
        else
            return false;
    }
    return true;
}

// Test buffer
bool Test_buffer(const std::string& filename) {
    uniform_int_distribution<int> u(1, 4095);
    default_random_engine e(
        chrono::steady_clock::now().time_since_epoch().count());

    for (int count = 0; count < 5; ++count) {
        Buffer buffer(filename);
        string lexeme;
        int line = 1;
        char c = buffer.current();
        if (c == '\n')
            ++line;
        do {
            for (int i = 0; i < u(e); ++i) {
                char c = buffer.forward();
                if (c == '\n')
                    ++line;
            }
            string temp = buffer.get_lexeme();
            // cout << temp;
            lexeme += temp;
        } while (buffer.is_forwardable());

        auto file_string = file_to_string(filename);

        if (lexeme == file_string)
            ;
        else
            return false;
    }

    for (int count = 0; count < 5; ++count) {
        Buffer buffer;
        buffer.load(filename);
        string lexeme;
        int line = 1;
        char c = buffer.current();
        if (c == '\n')
            ++line;
        do {
            for (int i = 0; i < u(e); ++i) {
                char c = buffer.forward();
                if (c == '\n')
                    ++line;
            }
            string temp = buffer.get_lexeme();
            // cout << temp;
            lexeme += temp;
        } while (buffer.is_forwardable());

        cout << line << endl;

        auto file_string = file_to_string(filename);

        if (lexeme == file_string)
            ;
        else
            return false;
    }

    return true;
}

// Test lexer_buffer_adapter
bool Test_lexer_buffer_adapter(const std::string& filename) {
    auto file_string = file_to_string(filename);
    uniform_int_distribution<int> u(1, 4095);
    default_random_engine e(
        chrono::steady_clock::now().time_since_epoch().count());

    for (int count = 0; count < 5; ++count) {
        LexerBufferAdapter buffer(filename);
        string lexeme;
        do {
            for (int i = 0; i < u(e); ++i) {
                buffer.forward();
            }
            buffer.set_token(0);
            auto temp = buffer.get_lexeme();
            lexeme += temp;
        } while (buffer.is_forwardable());

        if (lexeme == file_string)
            ;
        else
            return false;
    }
    for (int count = 0; count < 5; ++count) {
        LexerBufferAdapter buffer;
        buffer.load(filename);
        string lexeme;
        do {
            for (int i = 0; i < u(e); ++i) {
                buffer.forward();
            }
            buffer.set_token(0);
            auto temp = buffer.get_lexeme();
            lexeme += temp;
        } while (buffer.is_forwardable());

        if (lexeme == file_string)
            ;
        else
            return false;
    }
    return true;
}

void buffer_test() {
    try {
        //
        print_test_result(Test_buffer("buffer_test.h"));
        print_test_result(Test_lexer_buffer_adapter("buffer_test.h"));
        auto buffer_ptr = make_shared<LexerBufferAdapter>("buffer.h");
        print_test_result(
            Test_shared_ptr_buffer_adapter(buffer_ptr, "buffer_test.h"));
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
}

#endif // _ZCC_BUFFER_TEST_H_