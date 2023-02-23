
#ifndef _ZCC_STATEMENT_STRUCTURE_
#define _ZCC_STATEMENT_STRUCTURE_

#include "core/core.h"
#include <list>
#include <memory>
#include <vector>
// #include <zcc/code_generation/machine_architecture.h>
// #include <zcc/code_generation/register_allocation.h>

_CML_BEGIN_

class expression_tree;

class StatementStructureBase {
  protected:
    virtual std::string to_string() const { return ""; };

  public:
    // 2020/4/25 16:46 所有的语句自己生成代码
    // virtual std::list<IA32::Instruction> generate_code() {
    //     return std::list<IA32::Instruction>();
    // }
};

using ExpressionTree = std::shared_ptr<expression_tree>;
using StatementStructure = std::shared_ptr<StatementStructureBase>;

class IfStatementStructure : public StatementStructureBase {
  public:
    IfStatementStructure(ExpressionTree condition,
                         StatementStructure true_statement,
                         StatementStructure false_statement)
        : condition(condition), true_statement(true_statement),
          false_statement(false_statement) {}

  public:
    ExpressionTree condition;
    StatementStructure true_statement;
    StatementStructure false_statement;
};

std::shared_ptr<IfStatementStructure>
make_if(ExpressionTree condition, StatementStructure true_statement,
        StatementStructure false_statement);

std::shared_ptr<IfStatementStructure>
make_if(ExpressionTree condition, StatementStructure true_statement);

class IterationStatementStructure : public StatementStructureBase {};

class ForStatementStructure : public IterationStatementStructure {
  public:
    ForStatementStructure(ExpressionTree initialization,
                          ExpressionTree condition, ExpressionTree increment,
                          StatementStructure iteration_statement)
        : initialization(initialization), condition(condition),
          increment(increment), iteration_statement(iteration_statement) {}

  public:
    ExpressionTree initialization;
    ExpressionTree condition;
    ExpressionTree increment;
    StatementStructure iteration_statement;
};

std::shared_ptr<ForStatementStructure>
make_for(ExpressionTree initialization, ExpressionTree condition,
         ExpressionTree increment, StatementStructure iteration_statement);

class WhileStatementStructure : public IterationStatementStructure {
  public:
    WhileStatementStructure(ExpressionTree condition,
                            StatementStructure iteration_statement)
        : condition(condition), iteration_statement(iteration_statement) {}

  public:
    ExpressionTree condition;
    StatementStructure iteration_statement;
};

std::shared_ptr<WhileStatementStructure>
make_while(ExpressionTree condition, StatementStructure iteration_statement);

class DoWhileStatementStructure : public IterationStatementStructure {
  public:
    DoWhileStatementStructure(ExpressionTree condition,
                              StatementStructure iteration_statement)
        : condition(condition), iteration_statement(iteration_statement) {}

  public:
    ExpressionTree condition;
    StatementStructure iteration_statement;
};

std::shared_ptr<DoWhileStatementStructure>
make_do_while(ExpressionTree condition, StatementStructure iteration_statement);

class LabelStatementStructure : public StatementStructureBase {};

class CaseStatementStructure : public LabelStatementStructure {
  public:
    struct CaseLabel {
        int order;
        int value;
    };
    CaseStatementStructure(int order, int value,
                           StatementStructure case_statement)
        : case_label{order, value}, case_statement(case_statement) {}

  public:
    CaseLabel case_label;
    StatementStructure case_statement;
};

std::shared_ptr<CaseStatementStructure>
make_case(int order, int value, StatementStructure case_statement);

struct DefaultStatementStructure : public LabelStatementStructure {
  public:
    DefaultStatementStructure(StatementStructure default_statement)
        : default_statement(default_statement) {}

  public:
    StatementStructure default_statement;
};

std::shared_ptr<DefaultStatementStructure>
make_default(StatementStructure default_statement);

class SwitchStatementStructure : public StatementStructureBase {
  public:
    SwitchStatementStructure(
        ExpressionTree condition,
        std::shared_ptr<DefaultStatementStructure> default_label,
        std::vector<std::shared_ptr<CaseStatementStructure>> case_labels)
        : condition(condition), default_label(default_label),
          case_labels(case_labels) {}

  public:
    ExpressionTree condition;
    std::shared_ptr<DefaultStatementStructure> default_label;
    std::vector<std::shared_ptr<CaseStatementStructure>> case_labels;
};

std::shared_ptr<SwitchStatementStructure>
make_switch(ExpressionTree condition,
            std::shared_ptr<DefaultStatementStructure> default_label,
            std::vector<std::shared_ptr<CaseStatementStructure>> case_labels);

// 跳转语句有一个共同点 就是他们需要记录他们跳转到的地方
class JumpStatementStructure : public StatementStructureBase {};

class BreakStatementStructure : public JumpStatementStructure {
  public:
    BreakStatementStructure(StatementStructure statement_to_break)
        : statement_to_break(statement_to_break) {}

  public:
    StatementStructure statement_to_break;
};

std::shared_ptr<BreakStatementStructure>
make_break(StatementStructure statement_to_break);

// 这里应该是一个循环结构
class ContinueStatementStructure : public JumpStatementStructure {
  public:
    ContinueStatementStructure(
        std::shared_ptr<IterationStatementStructure> iteration_statement)
        : iteration_statement(iteration_statement) {}

  public:
    std::shared_ptr<IterationStatementStructure> iteration_statement;
};

std::shared_ptr<ContinueStatementStructure>
make_continue(std::shared_ptr<IterationStatementStructure> iteration_statement);

class ReturnStatementStructure : public JumpStatementStructure {
  public:
    ReturnStatementStructure(int function, ExpressionTree return_expression)
        : function(function), return_expression(return_expression) {}

  public:
    int function;
    ExpressionTree return_expression;
};

std::shared_ptr<ReturnStatementStructure>
make_return(int function, ExpressionTree return_expression);

class ExpressionStatementStructure : public StatementStructureBase {
  public:
    ExpressionStatementStructure(ExpressionTree expression_tree)
        : expression_tree(expression_tree) {}

  public:
    ExpressionTree expression_tree;
};

std::shared_ptr<ExpressionStatementStructure>
make_expression(ExpressionTree expression_tree);

class CompoundStatementStructure : public StatementStructureBase {
  public:
    CompoundStatementStructure(
        std::list<StatementStructure> compound_statements)
        : compound_statements(compound_statements) {}

  public:
    std::list<StatementStructure> compound_statements;
};

std::shared_ptr<CompoundStatementStructure>
make_compound(std::list<StatementStructure> compound_statements);

_CML_END_

#endif // _ZCC_STATEMENT_STRUCTURE_