#include "ast/statement_structure.h"

_CML_BEGIN_

std::shared_ptr<IfStatementStructure>
make_if(ExpressionTree condition, StatementStructure true_statement,
        StatementStructure false_statement) {
    return std::make_shared<IfStatementStructure>(condition, true_statement,
                                                  false_statement);
}

std::shared_ptr<IfStatementStructure>
make_if(ExpressionTree condition, StatementStructure true_statement) {
    return std::make_shared<IfStatementStructure>(condition, true_statement,
                                                  nullptr);
}

std::shared_ptr<ForStatementStructure>
make_for(ExpressionTree initialization, ExpressionTree condition,
         ExpressionTree increment, StatementStructure iteration_statement) {
    return std::make_shared<ForStatementStructure>(
        initialization, condition, increment, iteration_statement);
}

std::shared_ptr<WhileStatementStructure>
make_while(ExpressionTree condition, StatementStructure iteration_statement) {
    return std::make_shared<WhileStatementStructure>(condition,
                                                     iteration_statement);
}

std::shared_ptr<DoWhileStatementStructure>
make_do_while(ExpressionTree condition,
              StatementStructure iteration_statement) {
    return std::make_shared<DoWhileStatementStructure>(condition,
                                                       iteration_statement);
}

std::shared_ptr<CaseStatementStructure>
make_case(int order, int value, StatementStructure case_statement) {
    return std::make_shared<CaseStatementStructure>(order, value,
                                                    case_statement);
}

std::shared_ptr<DefaultStatementStructure>
make_default(StatementStructure default_statement) {
    return std::make_shared<DefaultStatementStructure>(default_statement);
}

std::shared_ptr<SwitchStatementStructure>
make_switch(ExpressionTree condition,
            std::shared_ptr<DefaultStatementStructure> default_label,
            std::vector<std::shared_ptr<CaseStatementStructure>> case_labels) {
    return std::make_shared<SwitchStatementStructure>(condition, default_label,
                                                      case_labels);
}

std::shared_ptr<BreakStatementStructure>
make_break(StatementStructure statement_to_break) {
    return std::make_shared<BreakStatementStructure>(statement_to_break);
}

std::shared_ptr<ContinueStatementStructure> make_continue(
    std::shared_ptr<IterationStatementStructure> iteration_statement) {
    return std::make_shared<ContinueStatementStructure>(iteration_statement);
}

std::shared_ptr<ReturnStatementStructure>
make_return(int function, ExpressionTree return_expression) {
    return std::make_shared<ReturnStatementStructure>(function,
                                                      return_expression);
}

std::shared_ptr<ExpressionStatementStructure>
make_expression(ExpressionTree expression_tree) {
    return std::make_shared<ExpressionStatementStructure>(expression_tree);
}

std::shared_ptr<CompoundStatementStructure>
make_compound(std::list<StatementStructure> compound_statements) {
    return std::make_shared<CompoundStatementStructure>(compound_statements);
}

_CML_END_
