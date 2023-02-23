/*
        2020/5/15 研究C关键字的完美哈希

        我的完美哈希函数生成算法研究成功！
        他需要生成两个哈希函数
        这两个哈希函数分别需要一个128*32的数组用来表示其对应于每一个字母的key
        然后需要生成两个数组 h1 h2
        然后生成一个数组 g
        然后生成两个哈希函数和最终的完美哈希函数
*/

#include "perfect_hash_generator.h"
#include "string_hash.h"
#include "util/util.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

StringHash strh;
// 先准备一个关键字数组

constexpr size_t keyword_size = 28;
unsigned int graph_size = 32;

// constexpr size_t keyword_size = 12;
// constexpr unsigned int graph_size = 15;

char* keyword[keyword_size] = {
    // 类型
    "auto",   "extern", "static",   "const",   "volatile",

    "void",   "signed", "unsigned", "char",    "int",      "short",
    "long",   "float",  "double",

    "struct", "union",

    "if",     "else",   "switch",   "default", "do",       "while",
    "for",    "break",  "continue", "goto",    "return",   "sizeof",

};

vector<unsigned int> h1{};
vector<unsigned int> h2{};

// vector<unsigned int> h1 = { 5,5,10,6,13,13,4,0,6,8,9,3 };
// vector<unsigned int> h2 = { 9,7,12,10,7,11,2,3,3,4,14,1 };

vector<int> g{};
vector<bool> check_g{};

// 定义所有函数的函数指针
using HashPtr = unsigned int (*)(char*);
// 定义哈希函数指针表
HashPtr hash_function_table[] = {SDBMHash, RSHash,   JSHash,  PJWHash,
                                 ELFHash,  BKDRHash, DJBHash, APHash};
constexpr unsigned int hash_function_size = 8;

void support(unsigned int v) {
    int adjacent_v = 0;
    for (size_t i = 0; i < keyword_size; i++) {
        // 找到节点v0的所有相邻节点
        if (h1[i] == v || h2[i] == v) {
            if (h1[i] == v)
                adjacent_v = h2[i];
            else if (h2[i] == v)
                adjacent_v = h1[i];

            // 如果找到的边没有被设置 就设置他 并修改flag
            // g[v1] = h - g[v0]
            if (!check_g[adjacent_v]) {
                g[adjacent_v] = int(i) - g[v];
                check_g[adjacent_v] = true;
                support(adjacent_v);
            }
        }
    }
}

// 用h1,h2两个数组表示一个图，其中h1[i],h2[i]表示图中的一条边
// 共有边keyword_size = 28个
// 共有节点graph_size = 32个
bool is_circle(const vector<unsigned int>& h1, const vector<unsigned int>& h2,
               unsigned int keyword_size, // size(h) = keyword_size
               unsigned int graph_size)   // value(h) = graph_size
{

    // 我们使用参数来调用这个函数 不然太麻烦了

    // 检查h1 h2组成的图是否有环
    vector<bool> edge{};
    edge.resize(keyword_size, true);
    bool flag = false;
    /*
    对于无向图

算法1

我们知道对于环1-2-3-4-1，每个节点的度都是2，基于此我们有如下算法（这是类似于有向图的拓扑排序）：

求出图中所有顶点的度，
删除图中所有度<=1的顶点以及与该顶点相关的边，把与这些边相关的顶点的度减一
如果还有度<=1的顶点重复步骤2
最后如果还存在未被删除的顶点，则表示有环；否则没有环

    */

    // 首先计算出所有节点的度
    vector<unsigned int> degree{};
    degree.resize(graph_size, 0);

    while (true) {
        // flag = false 表示本次迭代还没有删除边
        flag = false;

        // 重新计算所有节点的度
        fill(degree.begin(), degree.end(), 0);
        for (size_t i = 0; i < keyword_size; i++) {
            if (edge[i]) {
                ++degree[h1[i]];
                ++degree[h2[i]];
            }
        }

        // 删除图中所有度小于1的定点和相应的边
        for (size_t i = 0; i < graph_size; i++) {
            // degree[i] = 0 表示此节点被删除了
            if (degree[i] >= 0 && degree[i] <= 1) {
                // 找到此节点对应的边 因为他的度为1
                // 所以必然只存在一条边以此节点为端点
                for (size_t j = 0; j < keyword_size; j++) {
                    // 首先这条边没有被删除
                    // 并且这条边的端点中有一个
                    if (edge[j] && (h1[j] == i || h2[j] == i)) {
                        // 我们应该删除这条边
                        edge[j] = false;
                        // 表示我们删除了边
                        flag = true;
                        break;
                    }
                }
            }
        }

        // 如果在本次迭代中没有删除边 表示算法执行完毕 跳出
        if (!flag)
            break;
    }

    // 检查是否存在未被删除的结点
    // 其实只需要检查是否还有未被删除的边即可
    for (const auto& e : edge)
        if (e)
            return true;
    return false;
}

/*
0 : 39  18
1 : 0   2
2 : 26  1
3 : 15  12
4 : 19  23
5 : 21  1
6 : 4   39
7 : 33  25
8 : 11  10
9 : 25  41
10 : 29 9
11 : 40 30
12 : 42 17
13 : 2  6
14 : 34 38
15 : 22 0
16 : 11 41
17 : 23 12
18 : 37 3
19 : 6  36
20 : 10 8
21 : 34 27
22 : 13 22
23 : 28 10
24 : 38 42
25 : 20 19
26 : 13 16
27 : 13 11
*/

bool app_main(HashPtr hash1, HashPtr hash2) {
    // 计算映射g
    int v0 = 0;
    int v1 = 0;

    h1.resize(keyword_size, 0);
    h2.resize(keyword_size, 0);

    for (size_t i = 0; i < keyword_size; i++) {
        h1[i] = hash1(keyword[i]) % graph_size;
        h2[i] = hash2(keyword[i]) % graph_size;
    }

    for (size_t i = 0; i < keyword_size; i++) {
        cout << i << " : " << h1[i] << "\t" << h2[i] << endl;
    }

    // 检查是否有环 如果有环 那么就结束 否则就执行接下来的计算
    if (is_circle(h1, h2, keyword_size, graph_size)) {
        cout << "存在环" << endl;
        return false;
    }
    cout << "不存在环" << endl;

    // 检查生成的图是否存在环
    // cout << "是否存在环" << is_circle(h1, h2, keyword_size, graph_size) <<
    // endl; cout << "是否存在环" << is_circle(th1, th2, 12, 15) << endl;

    g.resize(graph_size);
    std::fill(g.begin(), g.end(), 0);

    check_g.resize(graph_size);
    std::fill(check_g.begin(), check_g.end(), false);

    bool is_all_checked = false;

    while (true) {
        // 如果所有点都被设置了 那么程序执行结束

        is_all_checked = true;
        // 寻找一个没有被设置的点
        for (size_t i = 0; i < graph_size; i++) {
            if (!check_g[i]) {
                v0 = i;
                is_all_checked = false;
                break;
            }
        }

        if (is_all_checked) {
            // 输出g
            for (size_t i = 0; i < graph_size; i++) {
                cout << i << " : " << g[i] << endl;
            }
            return true;
        } else {
            // 初始化为0
            g[v0] = 0;
            check_g[v0] = true;
            support(v0);
        }
    }
}

StringHash strhash1;
StringHash strhash2;
bool my_main() {

    strhash2.random_weight();

    // 计算映射g
    int v0 = 0;
    int v1 = 0;

    h1.resize(keyword_size, 0);
    h2.resize(keyword_size, 0);

    for (size_t i = 0; i < keyword_size; i++) {
        h1[i] = strhash1.my_hash(keyword[i]) % graph_size;
        h2[i] = strhash2.my_hash(keyword[i]) % graph_size;
    }

    // for (size_t i = 0; i < keyword_size; i++)
    //{
    //	cout << i << " : " << h1[i] << "\t" << h2[i] << endl;
    // }

    // 检查是否有环 如果有环 那么就结束 否则就执行接下来的计算
    if (is_circle(h1, h2, keyword_size, graph_size)) {
        // cout << "存在环" << endl;
        return false;
    }
    // cout << "不存在环" << endl;

    // 检查生成的图是否存在环
    // cout << "是否存在环" << is_circle(h1, h2, keyword_size, graph_size) <<
    // endl; cout << "是否存在环" << is_circle(th1, th2, 12, 15) << endl;

    g.resize(graph_size);
    std::fill(g.begin(), g.end(), 0);

    check_g.resize(graph_size);
    std::fill(check_g.begin(), check_g.end(), false);

    bool is_all_checked = false;

    while (true) {
        // 如果所有点都被设置了 那么程序执行结束

        is_all_checked = true;
        // 寻找一个没有被设置的点
        for (size_t i = 0; i < graph_size; i++) {
            if (!check_g[i]) {
                v0 = i;
                is_all_checked = false;
                break;
            }
        }

        if (is_all_checked) {
            // 输出g
            for (size_t i = 0; i < graph_size; i++) {
                // cout << i << " : " << g[i] << endl;
                // 要求所有的g>=0
                // if (g[i] < 0)
                //	return false;
            }
            return true;
        } else {
            // 初始化为0
            g[v0] = 0;
            check_g[v0] = true;
            support(v0);
        }
    }
}

bool is_keyword(char* str) {
    int index = g[strhash1.my_hash(str) % graph_size] +
                g[strhash2.my_hash(str) % graph_size];
    if (index >= 0 && index < keyword_size) {
        return std::strcmp(str, keyword[index]) == 0;
    }
    return false;
}

bool is_keyword(const std::string& str) {
    int index = g[strhash1.my_hash((char*)str.c_str()) % graph_size] +
                g[strhash2.my_hash((char*)str.c_str()) % graph_size];
    if (index >= 0 && index < keyword_size) {
        return str == string(keyword[index]);
    }
    return false;
    // return str == string(keyword[0]);
}

/*
        这个函数可以实现完美哈希算法
        2020/5/15 19：55封存
*/
int old_main() {
    // StringHash strhash1;
    // StringHash strhash2;

    //// 做必要的初始化工作
    // HashPtr hash1 = BKDRHash;
    // HashPtr hash2 = APHash;
    // graph_size = 43;
    // for (size_t i = 0; i < hash_function_size; i++)
    //{
    //	hash1 = hash_function_table[i];
    //	for (size_t j = 0; j < hash_function_size; j++)
    //	{
    //		// 在调用每一个函数之前初始化所有的数据
    //		// h1 h2 g check_g v1 v2
    //		hash2 = hash_function_table[j];
    //		if(i != j)
    //			app_main(hash1, hash2);
    //	}
    // }
    // while (!app_main(hash1, hash2))
    //{
    //	graph_size++;
    // }

    // cout << "graph size" << graph_size;

    // app_main(nullptr, nullptr);
    // std::default_random_engine
    // e(std::chrono::system_clock::now().time_since_epoch().count());
    // std::uniform_int_distribution<unsigned int> u(0, 128);
    // unsigned int r = u(e);
    //// 测试我自己写的哈希函数
    // while (true)
    //{

    //	r = u(e);
    //	cout << r << endl;
    //}

    int i = 0;
    while (!my_main()) {
        cout << i++ << endl;
    }

    cout << "成功" << endl;
    // 在这里输出结果
    for (size_t i = 0; i < keyword_size; i++) {
        cout << i << " : " << h1[i] << " " << h2[i] << endl;
    }

    // 在这里输出g
    for (size_t i = 0; i < graph_size; i++) {
        cout << i << " : " << g[i] << endl;
        // 要求所有的g>=0
        // if (g[i] < 0)
        //	return false;
    }

    // 在这里输出两个asciikey表
    for (size_t i = 0; i < 128; i++) {
        cout << i << " : " << strhash1.ascii_weight[i] << endl;
    }

    for (size_t i = 0; i < 128; i++) {
        cout << i << " : " << strhash2.ascii_weight[i] << endl;
    }

    // 那么我们如何验证这个哈希函数是正确的呢？
    // h(t) = g(h1(t)) + g(h2(t))
    for (size_t i = 0; i < keyword_size; i++) {
        cout << i << " : "
             << g[strhash1.my_hash(keyword[i]) % graph_size] +
                    g[strhash2.my_hash(keyword[i]) % graph_size]
             << endl;
        cout << "is key word" << is_keyword(keyword[i]) << endl;
    }

    // 随机生成别的字符串看看是不是keyword
    std::default_random_engine e(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned int> u(0, 20);
    int count = 100000;
    while (count-- > 0) {
        string str = util::make_random_string(u(e));
        // cout << str << " " << is_keyword(str) << endl;
        if (is_keyword(str)) {
            cout << str << endl;
            cout << str << "是关键字" << endl;
        }
    }
    cout << "测试成功！";
    return 0;
}

int main() {
    try {
        PerfectHashGenerator generator("keyword.txt");
        generator.generate_codefile();

        // 2020/5/21 多输出点东西
        cout << "输出字符串数组" << endl;
        for (size_t i = 0; i < keyword_size; i++) {
            cout << i << " : " << keyword[i] << endl;
        }

        cout << "输出h1,h2" << endl;
        for (size_t i = 0; i < generator.h1.size(); i++) {
            cout << i << " : " << keyword[i] << "\t\t" << generator.h1[i]
                 << "\t" << generator.h2[i] << endl;
        }

        cout << "输出g" << endl;
        for (size_t i = 0; i < generator.g.size(); i++) {
            cout << i << " : " << generator.g[i] << endl;
        }

        // 正确性测试
        for (size_t i = 0; i < generator.edge_size; i++) {
            // cout << i << " : " << g[strhash1.my_hash(keyword[i]) %
            // graph_size] + g[strhash2.my_hash(keyword[i]) % graph_size] <<
            // endl;
            cout << "is key word"
                 << generator.perfect_hash(generator.string_vector[i]) << endl;
        }

        // 随机生成别的字符串看看是不是keyword
        std::default_random_engine e(
            std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<unsigned int> u(0, 128);
        int count = 100000;
        while (count-- > 0) {
            string str = util::make_random_string(u(e));
            // cout << str << " " << is_keyword(str) << endl;
            if (generator.perfect_hash(str)) {
                cout << str << endl;
                cout << str << "是关键字" << endl;
            }
        }
        cout << "测试成功！";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
