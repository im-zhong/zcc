// 2020/2/29
// zhong

#include "util/util.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <type_traits>

namespace util {
// Do not use namespace using - directives;
// use using - declarations instead
// using namespace std;
using std::cout;
using std::endl;

std::string make_random_string(size_t length) {
    static std::uniform_int_distribution<int> u('A', 'z');
    static std::default_random_engine e(
        std::chrono::system_clock::now().time_since_epoch().count());

    std::string random_string;
    int r = 0;
    for (size_t i = 0U; i < length;) {
        r = u(e);
        if (!(r > 'Z' && r < 'a')) {
            random_string.push_back(static_cast<char>(r));
            ++i;
        }
    }

    // std::cout << random_string.capacity() << std::endl;
    return random_string;
}

std::string make_random_name() { return make_random_string(31); }

void print_test_result(bool result) {
    if (result) {
        cout << "测试成功" << endl;
    } else {
        cout << "测试失败" << endl;
    }
}

std::string file_to_string(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error(
            ERROR_MESSAGE(std::string{"无法打开文件："} + filename));
    }

    std::string file;
    char file_content[4096] = {};
    while (fin) {
        fin.read(file_content, 4096);
        auto size = fin.gcount();
        std::string temp(file_content, file_content + size);
        file += temp;
    }
    return file;
}

} // namespace util
