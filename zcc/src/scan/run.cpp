/*
        2020/4/25 11:05
        zhong, ysu
        CMake Lexer
*/

#include "lexer.h"
#include "make_terminal.h"

using namespace std;
using namespace cml;

int main() {
    try {
        std::shared_ptr<Terminal_Base> terminal;
        Lexer lex;
        if (!lex.load("../lexer_test.txt")) {
            cout << "file open failed." << endl;
        } else {
            cout << "file open successed." << endl;
        }
        system("pause");
        Token token;
        while (true) {
            token = lex.lexer();
            // 2020/5/23 不用变成terminal
            // terminal = make_terminal(token);
            // cout << "terminal category : " << token.category << endl;;
            if (token.category != 255)
                cout << lex.current_line() << " : " << token << endl;
            else {
                cout << "eof" << endl;
                break;
            }
        }
        cout << "all lexeme count : " << lex.lexeme_count() << endl;
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }

    system("pause");
    return 0;
}