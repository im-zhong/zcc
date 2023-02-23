

// 这个文件的划分需要各个子自动机之间的依赖图

#ifndef _ZCC_LEXER_AUTOMAT_IMPL_H_
#define _ZCC_LEXER_AUTOMAT_IMPL_H_

#include "lexer_automat_base.h"
// #include"buffer.h"

#include "convert.h"

#include "error.h"

#include "core/grammar_abstract.h"
#include <cstdlib>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

_CML_BEGIN_

//...
class Undefined : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        std::string msg = "undefined character : ";
        msg += buffer->get_peek();
        lexer_error()(msg);
    }
};

class Delimiter : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;

    Delimiter(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr) {
        std::vector<uint8_t> __delimiter_convert = {
            TERMINAL::DLM_SOE,   // (
            TERMINAL::DLM_EOE,   // )
            TERMINAL::DLM_COMMA, // ,
            TERMINAL::DLM_SEMIC, // ;
            TERMINAL::DLM_SOCE,  // [
            TERMINAL::DLM_EOCE,  // ]
            TERMINAL::DLM_SOS,   // {
            TERMINAL::DLM_EOS,   // }
        };
        delimiter_convert = __delimiter_convert;
    }
    virtual void run() override {
        auto category = get_sub_category(buffer->get_peek());
        buffer->forward();
        buffer->set_token(delimiter_convert[category]);
    }

  private:
    std::vector<uint8_t> delimiter_convert;
};

class WhiteSpace : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        for (; true; buffer->forward()) { // if the first char is newline,then i
                                          // will lose it
            // so do not call the forward at the first time
            if (buffer->is_peek({blank, tab}))
                continue;
            else if (buffer->is_peek(newline))
                buffer->inc_line();
            else
                break;
        }
        buffer->skip();
    }
};

class Identifier : public LexerAutomatBase {
  public:
    Identifier(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr) {
        // TODO 暂时先把这一块给填上吧
        keyword_table.insert({"auto", TERMINAL::RES_AUTO});
        keyword_table.insert({"break", TERMINAL::RES_BREAK});
        keyword_table.insert({"case", TERMINAL::RES_CASE});
        keyword_table.insert({"char", TERMINAL::RES_CHAR});
        keyword_table.insert({"const", TERMINAL::RES_CONST});
        keyword_table.insert({"continue", TERMINAL::RES_CONTINUE});
        keyword_table.insert({"default", TERMINAL::RES_DEFAULT});
        keyword_table.insert({"do", TERMINAL::RES_DO});
        keyword_table.insert({"double", TERMINAL::RES_DOUBLE});
        keyword_table.insert({"else", TERMINAL::RES_ELSE});
        keyword_table.insert({"enum", TERMINAL::RES_ENUM});
        keyword_table.insert({"extern", TERMINAL::RES_EXTERN});
        keyword_table.insert({"float", TERMINAL::RES_FLOAT});
        keyword_table.insert({"for", TERMINAL::RES_FOR});
        keyword_table.insert({"goto", TERMINAL::RES_GOTO});
        keyword_table.insert({"if", TERMINAL::RES_IF});
        keyword_table.insert({"int", TERMINAL::RES_INT});
        keyword_table.insert({"long", TERMINAL::RES_LONG});
        keyword_table.insert({"register", TERMINAL::RES_REGISTER});
        keyword_table.insert({"return", TERMINAL::RES_RETURN});
        keyword_table.insert({"short", TERMINAL::RES_SHORT});
        keyword_table.insert({"signed", TERMINAL::RES_SIGNED});
        keyword_table.insert({"sizeof", TERMINAL::RES_SIZEOF});
        keyword_table.insert({"static", TERMINAL::RES_STATIC});
        keyword_table.insert({"struct", TERMINAL::RES_STRUCT});
        keyword_table.insert({"switch", TERMINAL::RES_SWITCH});
        keyword_table.insert({"typedef", TERMINAL::RES_TYPEDEF});
        keyword_table.insert({"union", TERMINAL::RES_UNION});
        keyword_table.insert({"unsigned", TERMINAL::RES_UNSIGNED});
        keyword_table.insert({"void", TERMINAL::RES_VOID});
        keyword_table.insert({"volatile", TERMINAL::RES_VOLATILE});
        keyword_table.insert({"while", TERMINAL::RES_WHILE});
    }
    virtual void run() override {
        for (; isalnum(buffer->get_peek()) || buffer->is_peek('_');
             buffer->forward())
            ;

        // TODO 2020/2/3 这里要处理关键字的呀
        std::string lexeme = buffer->look_lexeme();
        if (keyword_table.find(lexeme) == keyword_table.end()) {
            buffer->set_token(TERMINAL::ID);
        } else {
            buffer->set_token(keyword_table[lexeme]);
        }
    }

    std::map<std::string, TERMINAL> keyword_table;
};

class LiteralScientific : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;

    virtual void run() override {
        buffer->forward();
        sign = false;
        if (buffer->is_peek({'+', '-'}))
            sign = true;
        if (sign) {
            buffer->forward();
            if (isdigit(buffer->get_peek()))
                for (; isdigit(buffer->get_peek()); buffer->forward())
                    ;
            else
                lexer_error()(
                    "scientific lietral's sign must follow with digit.");
        } else if (isdigit(buffer->get_peek()))
            for (; isdigit(buffer->get_peek()); buffer->forward())
                ;
        else
            lexer_error()(
                "scientific lietral must begin with '+' '-' or digit.");
        buffer->set_token(TERMINAL::LIT_REAL);
    }

  protected:
    bool sign = false;
};

class LiteralReal : public LexerAutomatBase {
  public:
    LiteralReal(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr),
          literal_scientific(std::make_shared<LiteralScientific>(buffer_ptr)) {}

    LiteralReal(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
                std::shared_ptr<LiteralScientific> __literal_scientific)
        : LexerAutomatBase(buffer_ptr),
          literal_scientific(__literal_scientific) {}

    virtual void run() override {
        if (!isdigit(buffer->get_peek()))
            lexer_error()("literal real must follow with digits");
        for (; isdigit(buffer->get_peek()); buffer->forward())
            ;
        if (buffer->is_peek({'e', 'E'}))
            literal_scientific->run();
        else
            buffer->set_token(TERMINAL::LIT_REAL);
    }

    std::shared_ptr<LiteralScientific> get_literal_scientific() const {
        return literal_scientific;
    }

  protected:
    std::shared_ptr<LiteralScientific> literal_scientific;
};

class LiteralInteger : public LexerAutomatBase {
  public:
    LiteralInteger(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr),
          literal_scientific(std::make_shared<LiteralScientific>(buffer_ptr)),
          literal_real(
              std::make_shared<LiteralReal>(buffer_ptr, literal_scientific)) {}

    LiteralInteger(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
                   std::shared_ptr<LiteralScientific> __literal_scientific,
                   std::shared_ptr<LiteralReal> __literal_real)
        : LexerAutomatBase(buffer_ptr),
          literal_scientific(__literal_scientific),
          literal_real(__literal_real) {}

    virtual void run() override {
        integer_count = 0;
        for (; isdigit(buffer->get_peek()); buffer->forward())
            ++integer_count;
        dispatch(buffer->get_peek());
    }

    virtual void dispatch(uint8_t __category) override {
        switch (__category) {
        case '.':
            literal_real->run();
            break;
        case 'e':
        case 'E':
            if (integer_count > 0)
                literal_scientific->run();
            else
                lexer_error()(
                    "scientific real must have a integer before (e|E)");
            break;
        default:
            buffer->set_token(TERMINAL::LIT_INT);
        }
    }

    std::shared_ptr<LiteralScientific> get_literal_scientific() const {
        return literal_scientific;
    }

    std::shared_ptr<LiteralReal> get_literal_real() const {
        return literal_real;
    }

  private:
    int integer_count = 0;
    std::shared_ptr<LiteralScientific> literal_scientific;
    std::shared_ptr<LiteralReal> literal_real;
};

class Escape : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        buffer->forward();
        buffer->get_peek() = get_escape(buffer->get_peek());
    }

  private:
    bool isdigit8(char c) const { return c >= '0' && c <= '7'; }

    char get_escape_8d() {
        count = 3;
        while (--count) {
            buffer->forward();
            if (isdigit8(buffer->get_peek()))
                continue;
            else
                lexer_error()("escape 8-digit format error.");
        }
        return 0;
    }

    char get_escape_x() {
        count = 2;
        while (--count) {
            buffer->forward();
            if (isxdigit(buffer->get_peek()))
                continue;
            else
                lexer_error()("escape 16-digit format error.");
        }
        return 0;
    }

    int count = 0;

  protected:
    char get_escape(char c) {
        switch (c) {
        case 'a':
            return 7;
        case 'b':
            return 8;
        case 'f':
            return 12;
        case 'n':
            return 10;
        case 'r':
            return 13;
        case 't':
            return 9;
        case 'v':
            return 11;
        case '\\':
            return 92;
        case '\'':
            return 39;
        case '"':
            return 34;
        case '?':
            return 63;
        case '0':
            return 0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            return get_escape_8d();
        case 'x':
            return get_escape_x();
        default:
            lexer_error()("escape char error");
            exit(EXIT_FAILURE);
        }
    }
};

class LiteralString : public LexerAutomatBase {
  public:
    LiteralString(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr),
          escape(std::make_shared<Escape>(buffer_ptr)) {}

    LiteralString(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
                  std::shared_ptr<Escape> __escape)
        : LexerAutomatBase(buffer_ptr), escape(__escape) {}

    virtual void run() override {
        lexeme.clear();
        for (buffer->forward(); !buffer->is_peek('"'); buffer->forward()) {
            if (buffer->get_peek() == '\\')
                escape->run();
            else if (buffer->is_peek('\n'))
                lexer_error()("unpaired \" in line");
            lexeme.push_back(buffer->get_peek());
        }
        buffer->forward();
        buffer->skip();
        buffer->set_token(TERMINAL::LIT_STR, lexeme);
    }

    std::shared_ptr<Escape> get_escape() const { return escape; }

  protected:
    std::shared_ptr<Escape> escape;
    std::string lexeme;
};

class LiteralChar : public LexerAutomatBase {
  public:
    LiteralChar(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr),
          escape(std::make_shared<Escape>(buffer_ptr)) {}

    LiteralChar(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
                std::shared_ptr<Escape> __escape)
        : LexerAutomatBase(buffer_ptr), escape(__escape) {}

    virtual void run() override {
        count = 0;
        lexeme.clear();
        for (buffer->forward(); !buffer->is_peek('\''); buffer->forward()) {
            if (buffer->get_peek() == '\\')
                escape->run();
            else if (buffer->is_peek('\n'))
                lexer_error()("unpaired ' in line");
            ++count;
            lexeme.push_back(buffer->get_peek());
        }
        buffer->forward();
        if (count != 1)
            lexer_error()("char count != 1");
        buffer->skip();
        buffer->set_token(TERMINAL::LIT_CHAR, lexeme);
    }

    std::shared_ptr<Escape> get_escape() const { return escape; }

  private:
    int count = 0;
    std::shared_ptr<Escape> escape;
    std::string lexeme;
};

class LineAnnotation : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        // 有可能读取到的下一个字符是EOF
        // 那么我们就要结束读取 不然会造成死循环
        for (; !buffer->is_peek({newline, eof}); buffer->forward())
            ;
        buffer->skip();
    }
};

class BlockAnnotation : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        for (buffer->forward();;) {
            if (buffer->get_peek() == '*') {
                buffer->forward();
                if (buffer->get_peek() == '/')
                    break;
            } else if (buffer->get_peek() == newline) {
                buffer->inc_line();
                buffer->forward();
            } else
                buffer->forward();
        }
        buffer->forward(); // 准备下一个peek
        buffer->skip();
    }
};

class Operator : public LexerAutomatBase {
  public:
    Operator(std::shared_ptr<LexerBufferAdapter> buffer_ptr)
        : LexerAutomatBase(buffer_ptr),
          literal_integer(std::make_shared<LiteralInteger>(buffer_ptr)),
          line_annotation(std::make_shared<LineAnnotation>(buffer_ptr)),
          block_annotation(std::make_shared<BlockAnnotation>(buffer_ptr)) {}

    Operator(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
             std::shared_ptr<LiteralInteger> __literal_integer)
        : LexerAutomatBase(buffer_ptr), literal_integer(__literal_integer),
          line_annotation(std::make_shared<LineAnnotation>(buffer_ptr)),
          block_annotation(std::make_shared<BlockAnnotation>(buffer_ptr)) {}

    Operator(std::shared_ptr<LexerBufferAdapter> buffer_ptr,
             std::shared_ptr<LiteralInteger> __literal_integer,
             std::shared_ptr<LineAnnotation> __line_annotation,
             std::shared_ptr<BlockAnnotation> __block_annotation)
        : LexerAutomatBase(buffer_ptr), literal_integer(__literal_integer),
          line_annotation(__line_annotation),
          block_annotation(__block_annotation) {}

    virtual void run() override {
        is_anno = false;
        is_sign = false;
        // cout << "call Operator.run start" << endl;
        // cout << "\t call " << buffer->get_peek() << endl;
        // char c = buffer->get_peek();
        dispatch(buffer->get_peek());
        // std::string lexeme = buffer->look_lexeme();
        // cout << "look lexeme : " << lexeme << endl;
        if (!is_anno && !is_sign)
            buffer->set_token(operator_map.at(buffer->look_lexeme()));
        // cout << "call Operator.run end" << endl;
    }

  private:
    virtual void dispatch(uint8_t __category) override {
        __category = get_sub_category(__category);
        (this->*(function_automat[__category]))();
    }

  private:
    bool is_anno = false;
    bool is_sign = false;

    bool is_sign_number(uint8_t __last_category) {
        return __last_category == TERMINAL::LIT_INT ||
               __last_category == TERMINAL::LIT_REAL ||
               __last_category == TERMINAL::ID;
    }

    std::map<std::string, uint8_t> operator_map = {
        {"++", OP_INC},  // ++ (increment)
        {"--", OP_DEC},  // -- (decrement)
        {"+", OP_ADD},   // +  (plus)
        {"-", OP_SUB},   // -  (substract)
        {"*", OP_MUL},   // *  (multi)
        {"/", OP_DIV},   // /  (division)
        {"%", OP_MOD},   // %  (mod)
        {"+=", OP_ADDA}, // += (plus assgin)
        {"-=", OP_SUBA}, // -= (substract assgin)
        {"*=", OP_MULA}, // *= (multi assgin)
        {"/=", OP_DIVA}, // /= (division assgin)
        {"%=", OP_MODA}, // %= (mod assgin)

        {"&", OP_AND},    // &  (bit and)
        {"|", OP_OR},     // |  (or)
        {"~", OP_NOT},    // ~  (not)
        {"^", OP_XOR},    // ^  (xor)
        {"<<", OP_SHL},   // << (left shift)
        {">>", OP_SHR},   // >> (right shift)
        {"&=", OP_BANDA}, // &= (bit and equal)
        {"|=", OP_ORA},   // |= (or assgin)
        {"^=", OP_XORA},  // ^= (xor assgin)
        {"<<=", OP_SHLA}, // <<=(left shift assgin)
        {">>=", OP_SHRA}, // >>=(right shift assgin)

        {">", OP_GRE},   // >  (greater than)
        {">=", OP_GREE}, // >= (greater than equal)
        {"<", OP_LES},   // <  (less than)
        {"<=", OP_LESE}, // <= (less than equal)
        {"==", OP_EQU},  // == (equal)
        {"!=", OP_NEQ},  // != (not equal)

        {"!", OP_LNOT},  // !  (logical not)
        {"&&", OP_LAND}, // && (logical and)
        {"||", OP_LOR},  // || (logical or)

        {"=", OP_ASN},   // =  (assgin)
        {".", OP_PER},   // .  (period)
        {"->", OP_ARR},  // -> (arrow)
        {":", OP_COL},   // :  (colon)
        {"?", OP_QUE},   // ?  (question)
        {"...", OP_ELL}, // ...(ellipsis)
    };                   // operator map

    void exclamation() {
        if (buffer->is_forward('='))
            buffer->forward();
    }
    void mod() { exclamation(); }
    void bit_and() { exclamation(); }
    void asterisk() { exclamation(); }
    void plus() {
        buffer->forward();
        if (buffer->is_peek({'=', '+'}))
            buffer->forward();
        else if (is_sign_number(buffer->get_last_category()) &&
                 (isdigit(buffer->get_peek()) || buffer->is_peek('.'))) {
            literal_integer->run();
            is_sign = true;
        }
    } // minus plus 如果检测到的不是op 也不应该调度operator_map
    void minus() {
        buffer->forward();
        if (buffer->is_peek({'=', '-'}))
            buffer->forward();
        else if (is_sign_number(buffer->get_last_category()) &&
                 (isdigit(buffer->get_peek()) || buffer->is_peek('.'))) {
            literal_integer->run();
            is_sign = true;
        }
    }
    void period() {
        buffer->forward();
        if (buffer->get_peek() == '.') {
            if (buffer->is_forward('.'))
                buffer->forward();
            else
                ; // error
        }
    }
    void slash() {
        buffer->forward();
        if (buffer->get_peek() == '=')
            buffer->forward();
        else if (buffer->get_peek() == '/') {
            is_anno = true;
            line_annotation->run();
        } else if (buffer->get_peek() == '*') {
            is_anno = true;
            block_annotation->run();
        }
    }
    void colon() { buffer->forward(); }
    void less_than() {
        if (buffer->is_forward('='))
            buffer->forward();
        else if (buffer->is_peek('<'))
            if (buffer->is_forward('='))
                buffer->forward();
    }
    void equal() { exclamation(); }
    void greater_than() {
        if (buffer->is_forward('='))
            buffer->forward();
        else if (buffer->is_peek('>'))
            if (buffer->is_forward('='))
                buffer->forward();
    }
    void question() { buffer->forward(); }
    void bit_xor() { exclamation(); }
    void bit_or() {
        if (buffer->is_forward({'|', '='}))
            buffer->forward();
    }
    void bit_not() {
        if (buffer->is_forward('='))
            buffer->forward();
    }

  public:
    std::shared_ptr<LiteralInteger> get_literal_integer() const {
        return literal_integer;
    }
    std::shared_ptr<LineAnnotation> get_line_annotation() const {
        return line_annotation;
    }
    std::shared_ptr<BlockAnnotation> get_block_annotation() const {
        return block_annotation;
    }

  protected:
    std::shared_ptr<LiteralInteger> literal_integer;
    std::shared_ptr<LineAnnotation> line_annotation;
    std::shared_ptr<BlockAnnotation> block_annotation;

    using Function_Automat = void (Operator::*)();
    std::vector<Function_Automat> function_automat = {
        &Operator::exclamation, &Operator::mod,     &Operator::bit_and,
        &Operator::asterisk,    &Operator::plus,    &Operator::minus,
        &Operator::period,      &Operator::slash,   &Operator::colon,
        &Operator::less_than,   &Operator::equal,   &Operator::greater_than,
        &Operator::question,    &Operator::bit_xor, &Operator::bit_or,
        &Operator::bit_not};

    // std::map<char, Function_Automat> function_automat =
    //{
    //	{'!',&Operator::exclamation},
    //{'%',&Operator::mod },
    //{'&',	&Operator::bit_and},
    //{'*',&Operator::asterisk },
    //{'+',	&Operator::plus},
    //{'-',		&Operator::minus },
    //{'.',	&Operator::period},
    //{'\\',	&Operator::slash },
    //{':',	&Operator::colon},
    //{'<',	&Operator::less_than },
    //{'=',	&Operator::equal},
    //{'>',	&Operator::greater_than },
    //{'?',	&Operator::question},
    //{'^',	&Operator::bit_xor },
    //{'|',	&Operator::bit_or},
    //{'~',	&Operator::bit_not },
    // };
};

class PreProcessor : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        // cout << "call PreProcessor.run start" << endl;
        // for (; isalpha(buffer->get_peek()); buffer->forward());
        buffer->forward();
        buffer->skip();
        // cout << "call PreProcessor.run end" << endl;
    }
};

class BackSlash : public LexerAutomatBase {
  public:
    using LexerAutomatBase::LexerAutomatBase;
    virtual void run() override {
        // 反斜杠忽略下一个字符 一般情况下会忽略回车
    }
};

_CML_END_
#endif // _ZCC_LEXER_AUTOMAT_IMPL_H_