
#ifndef _ZCC_STATEMENT_H_
#define _ZCC_STATEMENT_H_

#include "core/core.h"
#include "core/grammar_abstract.h"
#include "scan/token.h"
#include "statement_structure.h"
#include <set>
#include <vector>

_CML_BEGIN_

class StatementParser {
  public:
    static Token token;
    virtual StatementStructure operator()() = 0;

    bool is_expression(Token token);
    StatementStructure expression();

    Token next_token();

    bool next_token(Token token);

    bool next_token(const std::string& lexeme);
    bool next_token(uint8_t category);

    bool is_statement(Token token);

    static std::set<std::string> make_statement_first();

    // 静态对象是否可以用静态函数来创建
    // 然后move过来 岂不美哉
    static std::set<std::string> statement_first;

    static std::vector<Token> make_token_vector();
    static std::vector<Token> token_vector;
    static int token_index;
};

class Statement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class NullStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class ContinueStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class BreakStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class ReturnStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class WhileStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class DoWhileStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class ForStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class IFStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class SwitchStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class ExpressionStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class CaseStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class DefaultStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

class CompoundStatement : public StatementParser {
  public:
    // 通过 StatementParser 继承
    virtual StatementStructure operator()() override;
};

// 既然如此 我可以使用单例啊 在这里
static Statement statement;
static NullStatement null_statement;
static ContinueStatement continue_statement;
static BreakStatement break_statement;
static ReturnStatement return_statement;
static WhileStatement while_statement;
static DoWhileStatement do_while_statement;
static ForStatement for_statement;
static IFStatement if_statement;
static SwitchStatement switch_statement;
static ExpressionStatement expression_statement;
static CaseStatement case_statement;
static DefaultStatement default_statement;
static CompoundStatement compound_statement;

_CML_END_

#endif // _ZCC_STATEMENT_H_