#pragma once
/*
        2020/5/15 17：17
        完美哈希函数生成器
*/

#include "util/util.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class PerfectHashGenerator {
  public:
    static constexpr unsigned int ascii_size = 128;

    size_t min_string_length{};
    size_t max_string_length{};

    std::default_random_engine e{};
    std::uniform_int_distribution<unsigned int> u{};
    std::array<unsigned int, ascii_size> hash_1_weight{};
    std::array<unsigned int, ascii_size> hash_2_weight{};

    std::string filename;

    std::vector<std::string> string_vector{};
    unsigned int edge_size{};
    unsigned int node_size{};

    std::vector<unsigned int> h1{};
    std::vector<unsigned int> h2{};

    std::vector<bool> edge{};
    std::vector<unsigned int> degree{};

    std::vector<int> g{};
    std::vector<bool> g_checked{};

  public:
    PerfectHashGenerator(const std::string& filename)
        : min_string_length(std::numeric_limits<size_t>::max()),
          max_string_length(std::numeric_limits<size_t>::min()),
          e(std::chrono::system_clock::now().time_since_epoch().count()),
          u(0, ascii_size - 1), hash_1_weight{}, hash_2_weight{},
          filename((filename)) {
        random_weight(hash_1_weight);

        int i = 0;

        read(this->filename);
        edge_size = string_vector.size();
        node_size = util::round_up(edge_size, 8);

        h1.resize(edge_size, 0);
        h2.resize(edge_size, 0);

        edge.resize(edge_size, false);
        degree.resize(node_size, 0u);

        g.resize(node_size, 0);
        g_checked.resize(node_size, false);

        do_generate();
    }

    // 这个函数应该生成代码文件
    void generate_codefile() { write("perfect_hash.cpp"); }

    bool perfect_hash(const std::string& str) const {
        if (str.size() <= max_string_length &&
            str.size() >= min_string_length) {
            int hash_value = g[hash_1(str)] + g[hash_2(str)];
            if (hash_value >= 0 && hash_value < edge_size)
                return str == string_vector[hash_value];
        }
        return false;
    }

  private:
    std::string generate_main() {

        std::string result{};
        result += "int main(int argc, const char* argv[])\n"
                  "{\n"
                  "	test();\n"
                  "	return 0;\n"
                  "}\n";
        return result;
    }

    // 从文件加载字符串
    void read(const std::string& filename) {
        try {
            std::string str{};
            std::ifstream fin(filename);
            if (fin.is_open()) {
                while (fin >> str) {
                    string_vector.push_back(str);
                    if (str.size() > max_string_length)
                        max_string_length = str.size();
                    if (str.size() < min_string_length)
                        min_string_length = str.size();
                }
                fin.close();
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    // 我应该将这个复杂的函数分解成简单的函数 他们生成string 然后我再写到文件中
    /*
            unsigned int hash_1_weight = {
                    1,2,3,4,5,6,7,8,
                    1,2,...
            };
    */
    std::string generate_ascii_weight(
        const std::array<unsigned int, ascii_size>& ascii_weight,
        const std::string& weight_array_name) {
        size_t newline = 8;
        std::string result{};
        result +=
            "const unsigned int " + weight_array_name + "[ASCII_SIZE] = {\n";
        for (size_t i = 0; i < ascii_size; i++) {
            if (i % newline == 0)
                result += "\t";
            result += std::to_string(ascii_weight[i]) + ", ";
            if (i % newline == newline - 1)
                result += "\n";
        }
        result += "};\n";
        return result;
    }

    /*
        我只需要生成一个哈希函数 然后根据参数调用相应的权重表即可
            unsigned int hash(const std::string& str，const unsigned int*
       ascii_weight)
            {
                    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
                    unsigned int hash = 0;

                    for (const auto& c : str)
                    {
                            hash = hash * seed + ascii_weight[c];
                    }

                    return (hash & 0x7FFFFFFF);
            }
    */
    std::string generate_hash() {
        std::string result{};
        result += "unsigned int hash(const std::string& str, const unsigned "
                  "int* ascii_weight)\n";
        result += "{\n";
        result += "\tunsigned int seed = 131;\n";
        result += "\tunsigned int hash = 0;\n";
        result += "\tfor (const auto& c : str)\n";
        result += "\t\thash = hash * seed + ascii_weight[c];\n";
        result += "\treturn (hash & 0x7FFFFFFF);\n";
        result += "}\n";
        return result;
    }

    std::string generate_header() {
        std::string result{};
        result += "#include <chrono>\n"
                  "#include <iostream>\n"
                  "#include <random>\n"
                  "#include <string>\n";
        return result;
    }

    /*
            constexpr MAX_STRING_LENGTH;
            constexpr unsigned int MIN_STRING_LENGTH;
            constexpr unsigned int EDGE_SIZE; STRING_COUNT
            constexpr unsigned int NODE_SIZE; MAX_STRING_COUNT

            bool perfect_hash(const std::string& str) const
            {
                    if (str.size() <= max_string_length
                            && str.size() >= min_string_length)
                    {
                            int hash_value = g[hash_1(str)] + g[hash_2(str)];
                            if (hash_value >= 0 && hash_value < edge_size)
                                    return str == string_vector[hash_value];
                    }
                    return false;
            }
    */
    std::string generate_perfect_hash() {
        std::string result{};
        result += "bool perfect_hash(const std::string& str)\n";
        result += "{\n";
        result += "\tif (str.size() <= MAX_STRING_LENGTH && str.size() >= "
                  "MIN_STRING_LENGTH)\n";
        result += "\t{\n";
        result += "\t\tint hash_value = g[hash(str,hash_1_weight) % "
                  "MAX_STRING_COUNT] + g[hash(str,hash_2_weight) % "
                  "MAX_STRING_COUNT];\n";
        result += "\t\tif (hash_value >= 0 && hash_value < STRING_COUNT)\n";
        result += "\t\t\treturn str == keyword[hash_value];\n";
        result += "\t}\n";
        result += "\treturn false;\n";
        result += "}\n";
        return result;
    }

    std::string generate_constexpr() {
        std::string result{};
        result += "constexpr unsigned int ASCII_SIZE = " +
                  std::to_string(ascii_size) + ";\n";
        result += "constexpr unsigned int MAX_STRING_LENGTH = " +
                  std::to_string(max_string_length) + ";\n";
        result += "constexpr unsigned int MIN_STRING_LENGTH = " +
                  std::to_string(min_string_length) + ";\n";
        result += "constexpr unsigned int STRING_COUNT = " +
                  std::to_string(edge_size) + ";\n";
        result += "constexpr unsigned int MAX_STRING_COUNT = " +
                  std::to_string(node_size) + ";\n";
        return result;
    }

    /*
            const char* keyword[] = {
                    "asdfasd",
            };
    */
    std::string generate_string_array() {
        std::string result{};
        size_t newline = 4;
        result += "const char* keyword[STRING_COUNT] = {\n";
        for (size_t i = 0; i < string_vector.size(); i++) {
            if (i % newline == 0)
                result += "\t";
            result += "\"" + string_vector[i] + "\", ";
            if (i % newline == newline - 1)
                result += "\n";
        }
        result += "};\n";
        return result;
    }

    /*
            生成h1 h2 不对 h数组是不需要生成的 只需要生成g数组
            const int g[] = {
                    ...
            };
    */
    std::string generate_g() {
        std::string result{};
        size_t newline = 8;
        result += "const int g[MAX_STRING_COUNT] = {\n";
        for (size_t i = 0; i < g.size(); i++) {
            if (i % newline == 0)
                result += "\t";
            result += std::to_string(g[i]) + ", ";
            if (i % newline == newline - 1)
                result += "\n";
        }
        result += "};\n";
        return result;
    }

    void write(const std::string& filename) {
        std::string result{};
        result += generate_header();
        result += "\n";
        result += generate_constexpr();
        result += "\n";
        result += generate_string_array();
        result += "\n";
        result += generate_ascii_weight(hash_1_weight, "hash_1_weight");
        result += "\n";
        result += generate_ascii_weight(hash_2_weight, "hash_2_weight");
        result += "\n";
        result += generate_g();
        result += "\n";
        result += generate_hash();
        result += "\n";
        result += generate_perfect_hash();
        result += "\n";
        result += generate_make_random_string();
        result += "\n";
        result += generate_test();
        result += "\n";
        result += generate_main();
        std::ofstream fout(filename);
        fout << result;
        fout.close();
    }

    std::string generate_make_random_string() {
        std::string result{};
        result +=
            "std::string make_random_string(size_t length)\n"
            "{\n"
            "\tstatic std::uniform_int_distribution<int> u('A', 'z');\n"
            "\tstatic std::default_random_engine "
            "e(std::chrono::system_clock::now().time_since_epoch().count());\n"
            "\tstd::string random_string;\n"
            "\tint r = 0;\n"
            "\tfor (size_t i = 0u; i < length;)\n"
            "\t{\n"
            "\t\tr = u(e);\n"
            "\t\tif (!(r > 'Z' && r < 'a'))\n"
            "\t\t{\n"
            "\t\t\trandom_string.push_back(static_cast<char>(r));\n"
            "\t\t\t++i;\n"
            "\t\t}\n"
            "\t}\n"
            "\treturn random_string;\n"
            "}\n";
        return result;
    }

    std::string generate_test() {
        std::string result{};
        result += "void test()\n"
                  "{\n"
                  "	for (size_t i = 0; i < STRING_COUNT; i++)\n"
                  "	{\n"
                  "		if (!perfect_hash(keyword[i]))\n"
                  "		{\n"
                  "			std::cout << \"Keyword test FAILED.\" "
                  "<< std::endl;\n"
                  "			break;\n"
                  "		}\n"
                  "		else\n"
                  "		{\n"
                  "			std::cout << \"Test SUCCESS : \" << "
                  "keyword[i] << std::endl;\n"
                  "		}\n"
                  "	}\n"
                  "	std::cout << \"Keyword test SUCCESS!\" << std::endl;\n";
        result +=
            "	std::default_random_engine "
            "e(std::chrono::system_clock::now().time_since_epoch().count());\n"
            "	std::uniform_int_distribution<unsigned int> u(0, 32);\n"
            "	int count = 1000000;\n"
            "	std::string str{};\n"
            "	while (count-- > 0)\n"
            "	{\n"
            "		str = make_random_string(u(e));\n"
            "		if (perfect_hash(str))\n"
            "			std::cout <<\"Generate a keyword randomly : "
            "\"<< str << std::endl;\n"
            "	}\n"
            "	std::cout << \"Random string test SUCCESS!\";\n"
            "}\n";
        return result;
    }

    void do_generate() {
        while (!find_perfect_hash())
            ;
    }

    void random_weight(std::array<unsigned int, ascii_size>& ascii_weight) {
        unsigned int r = 0u;
        std::fill(ascii_weight.begin(), ascii_weight.end(),
                  std::numeric_limits<unsigned int>::max());

        for (size_t i = 0; i < ascii_size; ++i) {
            while (true) {
                r = u(e);
                if (ascii_weight[r] ==
                    std::numeric_limits<unsigned int>::max()) {
                    ascii_weight[r] = i;
                    break;
                }
            }
        }
    }

    unsigned int
    hash(const std::string& str,
         const std::array<unsigned int, ascii_size>& ascii_weight) const {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        for (const auto& c : str) {
            hash = hash * seed + ascii_weight[c];
        }

        return (hash & 0x7FFFFFFF);
    }

    bool find_perfect_hash() {
        random_weight(hash_2_weight);

        for (size_t i = 0; i < edge_size; i++) {
            h1[i] = hash_1(string_vector[i]);
            h2[i] = hash_2(string_vector[i]);
        }

        if (has_circle(h1, h2, edge_size, node_size))
            return false;

        std::fill(g.begin(), g.end(), 0);
        std::fill(g_checked.begin(), g_checked.end(), false);
        bool is_all_checked = true;
        unsigned int v = 0;

        // 计算映射g
        while (true) {
            is_all_checked = true;
            // 寻找一个没有被设置的点
            for (size_t i = 0; i < node_size; i++) {
                if (!g_checked[i]) {
                    v = i;
                    is_all_checked = false;
                    break;
                }
            }

            if (is_all_checked) {
                return true;
            } else {
                // 初始化为0
                g[v] = 0;
                g_checked[v] = true;
                find_perfect_hash_support(v);
            }
        }
    }

    // 这个函数的作用是递归的查找图然后修改g和g_checked 直到所有g都被设置
    void find_perfect_hash_support(unsigned int v) {
        unsigned int adjacency_v = 0u;
        for (size_t i = 0; i < edge_size; i++) {
            // 找到节点v的所有相邻节点
            if (h1[i] == v || h2[i] == v) {
                adjacency_v = h1[i] == v ? h2[i] : h1[i];

                // 如果找到的边没有被设置 就设置他 并修改is_delete_edge
                // g[adjacent_v] = (i) - g[v]
                if (!g_checked[adjacency_v]) {
                    g[adjacency_v] = static_cast<int>(i) - g[v];
                    g_checked[adjacency_v] = true;
                    find_perfect_hash_support(adjacency_v);
                }
            }
        }
    }

    // 用h1,h2两个数组表示一个图，其中h1[i],h2[i]表示图中的一条边
    // 共有边edge_size = 28个
    // 共有节点node_size = 32个
    /*
            求出图中所有顶点的度，
            删除图中所有度<=1的顶点以及与该顶点相关的边，把与这些边相关的顶点的度减一
            如果还有度<=1的顶点重复步骤2
            最后如果还存在未被删除的顶点，则表示有环；否则没有环
    */

    bool has_circle(const std::vector<unsigned int>& h1,
                    const std::vector<unsigned int>& h2, unsigned int edge_size,
                    unsigned int node_size) {
        std::fill(edge.begin(), edge.end(), true);
        std::fill(degree.begin(), degree.end(), 0u);
        for (size_t i = 0; i < edge_size; i++) {
            ++degree[h1[i]];
            ++degree[h2[i]];
        }

        bool is_delete_edge = false;
        while (true) {
            is_delete_edge = false;

            // 删除图中所有度小于1的定点和相应的边
            for (size_t i = 0; i < node_size; i++) {
                // degree[i] = 0 表示此节点被删除了
                if (degree[i] > 0 && degree[i] <= 1) {
                    // 找到此节点对应的边 因为他的度为1
                    // 所以必然只存在一条边以此节点为端点
                    for (size_t j = 0; j < edge_size; j++) {
                        // 首先这条边没有被删除
                        // 并且这条边的端点中有一个
                        if (edge[j] && (h1[j] == i || h2[j] == i)) {
                            // 我们应该删除这条边
                            edge[j] = false;
                            // 表示我们删除了边
                            is_delete_edge = true;
                            // 删除边之后 我们应该修改这条边连接着的两个端点的度
                            // h1[j] h2[j] 就是这两个端点
                            --degree[h1[j]];
                            --degree[h2[j]];
                            break;
                        }
                    }
                }
            }

            // 如果在本次迭代中没有删除边 表示算法执行完毕 跳出
            if (!is_delete_edge)
                break;
        }

        // 检查是否存在未被删除的结点
        // 其实只需要检查是否还有未被删除的边即可
        for (const auto& e : edge)
            if (e)
                return true;
        return false;
    }

    unsigned int hash_1(const std::string& str) const {
        return hash(str, hash_1_weight) % node_size;
    }

    unsigned int hash_2(const std::string& str) const {
        return hash(str, hash_2_weight) % node_size;
    }
};
