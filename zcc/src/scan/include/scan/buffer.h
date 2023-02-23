/*
        new buffer.h
        zhong, ysu
        2020/2/28

        TODO 修改所有文件包含头宏定义 写错了 尴尬
*/

#ifndef _ZCC_BUFFER_H_
#define _ZCC_BUFFER_H_

#include "core/core.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

_CML_BEGIN_

class Buffer final {
  public:
    Buffer() noexcept
        : filename(), fin(), front_buffer_pool{}, back_buffer_pool{},
          buffer(front_buffer_pool), back_buffer(back_buffer_pool),
          lexeme_begin(nullptr), lexeme_end(nullptr), forwardable(false),
          exchangeable(true) {}

    Buffer(const std::string& filename)
        : filename(filename),
          fin(filename), front_buffer_pool{}, back_buffer_pool{},
          buffer(front_buffer_pool), back_buffer(back_buffer_pool),
          lexeme_begin(nullptr), lexeme_end(nullptr), forwardable(false),
          exchangeable(true) {
        load(filename);
    }

    ~Buffer() { clear(); }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

  public:
    // TODO
    // 单例模式改为无参的样式 有参数会导致错误的理解
    // 单例模式
    // static Buffer& make_buffer(const std::string& filename)
    //{
    //	// local static 优雅而牛逼的单例实现
    //	static Buffer buffer(filename);
    //	return buffer;
    //}

    // static Buffer& make_buffer() noexcept
    //{
    //	static Buffer buffer;
    //	return buffer;
    // }

    static std::unique_ptr<Buffer> make_buffer() noexcept {
        // 智能指针的移动会不会伴随其内部值的移动
        // 我禁止了移动啊
        return std::make_unique<Buffer>();
    }

    static std::unique_ptr<Buffer> make_buffer(std::string filename) {
        return std::make_unique<Buffer>(std::move(filename));
    }

    //
    bool is_eof() const noexcept { return fin.eof(); }

    //
    std::string get_filename() const noexcept { return filename; }
    constexpr static size_t get_buffer_size() noexcept { return buffer_size; }

    //
    std::size_t get_lexeme_length() const noexcept {
        if (buffer && back_buffer && lexeme_begin && lexeme_end) {
            if (exchangeable)
                return lexeme_end - lexeme_begin;
            else
                return lexeme_end - buffer + back_buffer + buffer_size - 1 -
                       lexeme_begin;
        } else
            return 0;
    }

    bool is_forwardable() const noexcept { return forwardable; }

    // 清空Buffer
    void clear() {
        filename.clear();
        fin.close();
        lexeme_begin = nullptr;
        lexeme_end = nullptr;
        forwardable = false;
        exchangeable = true;
    }

    //
    void load(const std::string& filename) {
        // 如果加载文件发生失败 我们应该保证fin指向原来的文件
        std::ifstream fin_temp(filename);
        // 如果我们读到了一个空文件 虽然文件打开了 但是文件是空的
        // 所有文件状态也是无效 这里写错了 竟然又又yyyyyyyy是buffer写错了
        // 这个小东西写了一天 改bug改了一年了
        if (!fin_temp)
            throw std::runtime_error(ERROR_MESSAGE("加载文件发生错误。"));
        // 如果发生失败 类的任何状态都不会改变

        this->filename = filename;
        fin = std::move(fin_temp);

        load_buffer();

        lexeme_begin = buffer;
        lexeme_end = buffer;
        exchangeable = true;
    }

    //
    char current() const noexcept {
        if (lexeme_begin)
            return *lexeme_begin;
        else
            return eof;
    };

    //
    char forward() {
        // 这里应该是一个forwardable
        // 只有当lexeme_end触碰到文件的eof时，forwardable=false;
        if (forwardable) {
            // fin.eof = false;
            ++lexeme_end;
            if (*lexeme_end != eof) {
                return *lexeme_end;
            } else if (fin.eof()) {
                forwardable = false;
                return eof;
            } else {
                // 在这里fin有没有可能失效？
                exchange();
                load_buffer();
                lexeme_end = buffer;

                // bug 2020/2/29 18：25
                // 在考虑了一旦读入就是一个EOF的情况下
                // 修改了load函数的bug
                // 这里可以不用考虑这种情况
                // 这里必须要判断lexeme_end是否是eof
                // if (*lexeme_end == eof)
                //	forwardable = false;
                return *lexeme_end;
            }
        } else {
            return eof;
        }
    }

    //
    bool is_forward(char c) { return c == forward(); }

    // lexeme: [begin,end)
    // end在极限情况下会指向EOF
    // 不会自动追加字符串结束符
    std::string get_lexeme() {
        std::string lexeme;
        while (lexeme_begin != lexeme_end) {
            if (*lexeme_begin != eof)
                lexeme.push_back(*lexeme_begin++);
            else {
                // 如果碰到了eof说明在forward过程中发生了缓冲区交换
                lexeme_begin = buffer;
                // begin指针跳出了旧缓冲区 旧缓冲区可以读取新的数据了
                exchangeable = true;
            }
        }
        // lexeme.push_back('\0');
        return lexeme;
    }

    std::string look_lexeme() const {
        std::string lexeme;
        char* begin = lexeme_begin;
        while (begin != lexeme_end) {
            if (*lexeme_begin != eof)
                lexeme.push_back(*begin++);
            else
                begin = buffer;
        }
        return lexeme;
    }

    //
    void skip_lexeme() noexcept {
        while (lexeme_begin != lexeme_end) {
            if (*lexeme_begin != eof)
                ++lexeme_begin;
            else {
                lexeme_begin = buffer;
                exchangeable = true;
            }
        }
    }

  private:
    //
    void load_buffer() {
        if (!fin)
            throw std::runtime_error("加载文件时发生异常。");
        fin.read(buffer, buffer_size - 1);
        auto size = fin.gcount();
        buffer[size] = eof;
        // 读取完毕之后 如果没有读出任何数据 那么不能向前
        if (size == 0)
            forwardable = false;
        else
            forwardable = true;
    }

    //
    void exchange() {
        if (exchangeable) {
            std::swap(buffer, back_buffer);
            exchangeable = false;
        } else
            throw std::runtime_error("单一词素的长度大于4096");
    }

  public:
    //
    constexpr static char eof = EOF;
    constexpr static std::size_t buffer_size = 4096;

  private:
    //
    std::string filename;
    std::ifstream fin;

    //
    char front_buffer_pool[buffer_size];
    char back_buffer_pool[buffer_size];
    char* buffer;
    char* back_buffer;

    // 这两个可以指定一个词素
    // [begin, end)
    char* lexeme_begin;
    char* lexeme_end;

    //
    bool forwardable;
    bool exchangeable;
};

_CML_END_
#endif // _ZCC_BUFFER_H_
