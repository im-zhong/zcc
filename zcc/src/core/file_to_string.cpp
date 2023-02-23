#include "core/core.h"
#include <fstream>
#include <string>

using namespace std;

_CML_BEGIN_

std::string file_to_string(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin)
        throw std::runtime_error(ERROR_MESSAGE("无法打开文件：" + filename));

    string file;
    char file_content[4096] = {};
    while (fin) {
        fin.read(file_content, 4096);
        auto size = fin.gcount();
        string temp(file_content, file_content + size);
        file += temp;
    }
    return file;
}

_CML_END_