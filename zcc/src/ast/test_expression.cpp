#include "ast/expression.h"
#include "core/grammar_abstract.h"
using namespace std;
using namespace cml;

int main() {
    static ExpressionParser expression_parser;
    expression_parser.init();
    ExpressionParser::token = expression_parser.next_token();
    auto node = expression();

    auto tree = make_tree<string>();
    tree->set_root(node);

    for (auto& node : tree->postorder()) {
        cout << node->get_value();
    }
}