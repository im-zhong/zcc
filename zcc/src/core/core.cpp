#include "core/core.h"
#include <chrono>
#include <iostream>
#include <random>
#include <type_traits>

using namespace std;

_CML_BEGIN_

std::string make_random_string(size_t length) {
    static std::uniform_int_distribution<int> u('A', 'z');
    static std::default_random_engine e(
        std::chrono::system_clock::now().time_since_epoch().count());

    std::string random_string;
    int r = 0;
    for (size_t i = 0u; i < length;) {
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
    if (result)
        cout << "测试成功" << endl;
    else
        cout << "测试失败" << endl;
}

_CML_END_