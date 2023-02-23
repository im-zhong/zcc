
#ifndef _ZCC_LEXER_TEST_H_
#define _ZCC_LEXER_TEST_H_

#include "make_terminal.h"
#include <chrono>
#include <random>

using namespace std;

_CML_BEGIN_

// file to string
// std::string file_to_string(const std::string& filename)
//{
//	std::ifstream fin(filename);
//	if (!fin)
//		throw std::runtime_error(ERROR_MESSAGE("无法打开文件：" +
// filename)); 	string file; 	char file_content[4096] = {}; 	while (fin)
//	{
//		fin.read(file_content, 4096);
//		auto size = fin.gcount();
//		string temp(file_content, file_content + size);
//		file += temp;
//	}
//	return file;
//}
//
// void print_test_result(bool result)
//{
//	if (result)
//		cout << "测试成功" << endl;
//	else
//		cout << "测试失败" << endl;
//}

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

// Test lexer
void Test_lexer_1(const std::string& filename) {
    try {
        Lexer lexer;
        lexer.load(filename);
        while (!lexer.is_eof()) {
            auto token = lexer.lexer();
            cout << token << endl;
        }
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
}

void Test_lexer_2(const std::string& filename) {
    Lexer lexer;
    lexer.load(filename);

    // 测试lexer的底层adapter
    print_test_result(Test_shared_ptr_buffer_adapter(lexer.buffer, filename));
    // for (int i = 0; i < 10; ++i)
    //{
    //	auto token = lexer.lexer(); // 这个名字改成 get_token
    //	cout << token << endl;
    // }
}

bool Test_lexer_3(const std::string& filename) {
    try {
        std::shared_ptr<Terminal_Base> terminal;
        Lexer lex;
        if (!lex.load(filename)) {
            cout << "file open failed." << endl;
        } else {
            cout << "file open successed." << endl;
        }
        system("pause");
        Token token;
        while (true) {
            token = lex.lexer();
            terminal = make_terminal(token);
            cout << "terminal category : " << terminal->category << endl;
            ;
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

int lexer_test() {
    Test_lexer_1("Test.txt");
    Test_lexer_2("Test.txt");
    // cout << "hello,world" << endl;
    Test_lexer_3("lexer_automat_impl.h");
    system("pause");
    return 0;
}

_CML_END_

#endif // _ZCC_LEXER_TEST_H_