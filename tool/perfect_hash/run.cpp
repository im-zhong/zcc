/*
        2020/5/15 �о�C�ؼ��ֵ�������ϣ

        �ҵ�������ϣ���������㷨�о��ɹ���
        ����Ҫ����������ϣ����
        ��������ϣ�����ֱ���Ҫһ��128*32������������ʾ���Ӧ��ÿһ����ĸ��key
        Ȼ����Ҫ������������ h1 h2
        Ȼ������һ������ g
        Ȼ������������ϣ���������յ�������ϣ����
*/

#include "perfect_hash_generator.h"
#include "string_hash.h"
#include "util/util.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

StringHash strh;
// ��׼��һ���ؼ�������

constexpr size_t keyword_size = 28;
unsigned int graph_size = 32;

// constexpr size_t keyword_size = 12;
// constexpr unsigned int graph_size = 15;

char* keyword[keyword_size] = {
    // ����
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

// �������к����ĺ���ָ��
using HashPtr = unsigned int (*)(char*);
// �����ϣ����ָ���
HashPtr hash_function_table[] = {SDBMHash, RSHash,   JSHash,  PJWHash,
                                 ELFHash,  BKDRHash, DJBHash, APHash};
constexpr unsigned int hash_function_size = 8;

void support(unsigned int v) {
    int adjacent_v = 0;
    for (size_t i = 0; i < keyword_size; i++) {
        // �ҵ��ڵ�v0���������ڽڵ�
        if (h1[i] == v || h2[i] == v) {
            if (h1[i] == v)
                adjacent_v = h2[i];
            else if (h2[i] == v)
                adjacent_v = h1[i];

            // ����ҵ��ı�û�б����� ��������
            // ���޸�flag g[v1] = h - g[v0]
            if (!check_g[adjacent_v]) {
                g[adjacent_v] = int(i) - g[v];
                check_g[adjacent_v] = true;
                support(adjacent_v);
            }
        }
    }
}

// ��h1,h2���������ʾһ��ͼ������h1[i],h2[i]��ʾͼ�е�һ����
// ���б�keyword_size = 28��
// ���нڵ�graph_size = 32��
bool is_circle(const vector<unsigned int>& h1, const vector<unsigned int>& h2,
               unsigned int keyword_size, // size(h) = keyword_size
               unsigned int graph_size)   // value(h) = graph_size
{

    // ����ʹ�ò���������������� ��Ȼ̫�鷳��

    // ���h1 h2��ɵ�ͼ�Ƿ��л�
    vector<bool> edge{};
    edge.resize(keyword_size, true);
    bool flag = false;
    /*
    ��������ͼ

�㷨1

����֪�����ڻ�1-2-3-4-1��ÿ���ڵ�Ķȶ���2�����ڴ������������㷨����������������ͼ���������򣩣�

���ͼ�����ж���Ķȣ�
ɾ��ͼ�����ж�<=1�Ķ����Լ���ö�����صıߣ�������Щ����صĶ���Ķȼ�һ
������ж�<=1�Ķ����ظ�����2
������������δ��ɾ���Ķ��㣬���ʾ�л�������û�л�

    */

    // ���ȼ�������нڵ�Ķ�
    vector<unsigned int> degree{};
    degree.resize(graph_size, 0);

    while (true) {
        // flag = false ��ʾ���ε�����û��ɾ����
        flag = false;

        // ���¼������нڵ�Ķ�
        fill(degree.begin(), degree.end(), 0);
        for (size_t i = 0; i < keyword_size; i++) {
            if (edge[i]) {
                ++degree[h1[i]];
                ++degree[h2[i]];
            }
        }

        // ɾ��ͼ�����ж�С��1�Ķ������Ӧ�ı�
        for (size_t i = 0; i < graph_size; i++) {
            // degree[i] = 0 ��ʾ�˽ڵ㱻ɾ����
            if (degree[i] >= 0 && degree[i] <= 1) {
                // �ҵ��˽ڵ��Ӧ�ı� ��Ϊ���Ķ�Ϊ1 ���Ա�Ȼֻ����һ�����Դ˽ڵ�Ϊ�˵�
                for (size_t j = 0; j < keyword_size; j++) {
                    // ����������û�б�ɾ��
                    // ���������ߵĶ˵�����һ��
                    if (edge[j] && (h1[j] == i || h2[j] == i)) {
                        // ����Ӧ��ɾ��������
                        edge[j] = false;
                        // ��ʾ����ɾ���˱�
                        flag = true;
                        break;
                    }
                }
            }
        }

        // ����ڱ��ε�����û��ɾ���� ��ʾ�㷨ִ����� ����
        if (!flag)
            break;
    }

    // ����Ƿ����δ��ɾ���Ľ��
    // ��ʵֻ��Ҫ����Ƿ���δ��ɾ���ı߼���
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
    // ����ӳ��g
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

    // ����Ƿ��л� ����л� ��ô�ͽ��� �����ִ�н������ļ���
    if (is_circle(h1, h2, keyword_size, graph_size)) {
        cout << "���ڻ�" << endl;
        return false;
    }
    cout << "�����ڻ�" << endl;

    // ������ɵ�ͼ�Ƿ���ڻ�
    // cout << "�Ƿ���ڻ�" << is_circle(h1, h2, keyword_size, graph_size) << endl;
    // cout << "�Ƿ���ڻ�" << is_circle(th1, th2, 12, 15) << endl;

    g.resize(graph_size);
    std::fill(g.begin(), g.end(), 0);

    check_g.resize(graph_size);
    std::fill(check_g.begin(), check_g.end(), false);

    bool is_all_checked = false;

    while (true) {
        // ������е㶼�������� ��ô����ִ�н���

        is_all_checked = true;
        // Ѱ��һ��û�б����õĵ�
        for (size_t i = 0; i < graph_size; i++) {
            if (!check_g[i]) {
                v0 = i;
                is_all_checked = false;
                break;
            }
        }

        if (is_all_checked) {
            // ���g
            for (size_t i = 0; i < graph_size; i++) {
                cout << i << " : " << g[i] << endl;
            }
            return true;
        } else {
            // ��ʼ��Ϊ0
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

    // ����ӳ��g
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

    // ����Ƿ��л� ����л� ��ô�ͽ��� �����ִ�н������ļ���
    if (is_circle(h1, h2, keyword_size, graph_size)) {
        // cout << "���ڻ�" << endl;
        return false;
    }
    // cout << "�����ڻ�" << endl;

    // ������ɵ�ͼ�Ƿ���ڻ�
    // cout << "�Ƿ���ڻ�" << is_circle(h1, h2, keyword_size, graph_size) << endl;
    // cout << "�Ƿ���ڻ�" << is_circle(th1, th2, 12, 15) << endl;

    g.resize(graph_size);
    std::fill(g.begin(), g.end(), 0);

    check_g.resize(graph_size);
    std::fill(check_g.begin(), check_g.end(), false);

    bool is_all_checked = false;

    while (true) {
        // ������е㶼�������� ��ô����ִ�н���

        is_all_checked = true;
        // Ѱ��һ��û�б����õĵ�
        for (size_t i = 0; i < graph_size; i++) {
            if (!check_g[i]) {
                v0 = i;
                is_all_checked = false;
                break;
            }
        }

        if (is_all_checked) {
            // ���g
            for (size_t i = 0; i < graph_size; i++) {
                // cout << i << " : " << g[i] << endl;
                // Ҫ�����е�g>=0
                // if (g[i] < 0)
                //	return false;
            }
            return true;
        } else {
            // ��ʼ��Ϊ0
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
        �����������ʵ��������ϣ�㷨
        2020/5/15 19��55���
*/
int old_main() {
    // StringHash strhash1;
    // StringHash strhash2;

    //// ����Ҫ�ĳ�ʼ������
    // HashPtr hash1 = BKDRHash;
    // HashPtr hash2 = APHash;
    // graph_size = 43;
    // for (size_t i = 0; i < hash_function_size; i++)
    //{
    //	hash1 = hash_function_table[i];
    //	for (size_t j = 0; j < hash_function_size; j++)
    //	{
    //		// �ڵ���ÿһ������֮ǰ��ʼ�����е�����
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
    //// �������Լ�д�Ĺ�ϣ����
    // while (true)
    //{

    //	r = u(e);
    //	cout << r << endl;
    //}

    int i = 0;
    while (!my_main()) {
        cout << i++ << endl;
    }

    cout << "�ɹ�" << endl;
    // ������������
    for (size_t i = 0; i < keyword_size; i++) {
        cout << i << " : " << h1[i] << " " << h2[i] << endl;
    }

    // ���������g
    for (size_t i = 0; i < graph_size; i++) {
        cout << i << " : " << g[i] << endl;
        // Ҫ�����е�g>=0
        // if (g[i] < 0)
        //	return false;
    }

    // �������������asciikey��
    for (size_t i = 0; i < 128; i++) {
        cout << i << " : " << strhash1.ascii_weight[i] << endl;
    }

    for (size_t i = 0; i < 128; i++) {
        cout << i << " : " << strhash2.ascii_weight[i] << endl;
    }

    // ��ô���������֤�����ϣ��������ȷ���أ�
    // h(t) = g(h1(t)) + g(h2(t))
    for (size_t i = 0; i < keyword_size; i++) {
        cout << i << " : "
             << g[strhash1.my_hash(keyword[i]) % graph_size] +
                    g[strhash2.my_hash(keyword[i]) % graph_size]
             << endl;
        cout << "is key word" << is_keyword(keyword[i]) << endl;
    }

    // ������ɱ���ַ��������ǲ���keyword
    std::default_random_engine e(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned int> u(0, 20);
    int count = 100000;
    while (count-- > 0) {
        string str = util::make_random_string(u(e));
        // cout << str << " " << is_keyword(str) << endl;
        if (is_keyword(str)) {
            cout << str << endl;
            cout << str << "�ǹؼ���" << endl;
        }
    }
    cout << "���Գɹ���";
    return 0;
}

int main() {
    try {
        PerfectHashGenerator generator("keyword.txt");
        generator.generate_codefile();

        // 2020/5/21 ������㶫��
        cout << "����ַ�������" << endl;
        for (size_t i = 0; i < keyword_size; i++) {
            cout << i << " : " << keyword[i] << endl;
        }

        cout << "���h1,h2" << endl;
        for (size_t i = 0; i < generator.h1.size(); i++) {
            cout << i << " : " << keyword[i] << "\t\t" << generator.h1[i]
                 << "\t" << generator.h2[i] << endl;
        }

        cout << "���g" << endl;
        for (size_t i = 0; i < generator.g.size(); i++) {
            cout << i << " : " << generator.g[i] << endl;
        }

        // ��ȷ�Բ���
        for (size_t i = 0; i < generator.edge_size; i++) {
            // cout << i << " : " << g[strhash1.my_hash(keyword[i]) %
            // graph_size] + g[strhash2.my_hash(keyword[i]) % graph_size] <<
            // endl;
            cout << "is key word"
                 << generator.perfect_hash(generator.string_vector[i]) << endl;
        }

        // ������ɱ���ַ��������ǲ���keyword
        std::default_random_engine e(
            std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<unsigned int> u(0, 128);
        int count = 100000;
        while (count-- > 0) {
            string str = util::make_random_string(u(e));
            // cout << str << " " << is_keyword(str) << endl;
            if (generator.perfect_hash(str)) {
                cout << str << endl;
                cout << str << "�ǹؼ���" << endl;
            }
        }
        cout << "���Գɹ���";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
