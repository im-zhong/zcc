/*

*/

#include <chrono>
#include <iostream>
#include <random>
#include <string>

class StringHash {
  public:
    StringHash() { random_weight(); }

    // BKDR Hash
    unsigned int BKDRHash(char* str) {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        while (*str) {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    // 设计一种专为生成完美哈希而设计的字符串哈希函数
    // 为每一个字母设计一个key
    // 每次都随机生成
    // 不断测试 直到测试出一个可以使用的哈希函数
    int ascii_weight[128]{};
    // 将0 - 128 个数字重新填入这128个空间中 并且保证每个数字都是随机的

    void random_weight() {
        std::default_random_engine e(std::chrono::high_resolution_clock::now()
                                         .time_since_epoch()
                                         .count());
        std::uniform_int_distribution<unsigned int> u(0, 128);
        unsigned int r = 0;

        std::fill(std::begin(ascii_weight), std::end(ascii_weight), -1);

        for (size_t i = 0; i < 128; i++) {
            while (true) {
                r = u(e);
                if (ascii_weight[r] == -1) {
                    ascii_weight[r] = i;
                    break;
                }
            }
        }
    }

    unsigned int my_hash(char* str) {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        while (*str) {
            hash = hash * seed + (ascii_weight[*str++]);
        }

        return (hash & 0x7FFFFFFF);
    }

    // 常用字符串哈希函数有BKDRHash，APHash，DJBHash，JSHash，RSHash，SDBMHash，PJWHash，ELFHash等等。
    // SDBMHash
    unsigned int SDBMHash(char* str) {
        unsigned int hash = 0;

        while (*str) {
            // equivalent to: hash = 65599*hash + (*str++);
            hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        }

        return (hash & 0x7FFFFFFF);
    }

    // RS Hash
    unsigned int RSHash(char* str) {
        unsigned int b = 378551;
        unsigned int a = 63689;
        unsigned int hash = 0;

        while (*str) {
            hash = hash * a + (*str++);
            a *= b;
        }

        return (hash & 0x7FFFFFFF);
    }

    // JS Hash
    unsigned int JSHash(char* str) {
        unsigned int hash = 1315423911;

        while (*str) {
            hash ^= ((hash << 5) + (*str++) + (hash >> 2));
        }

        return (hash & 0x7FFFFFFF);
    }

    // P. J. Weinberger Hash
    unsigned int PJWHash(char* str) {
        unsigned int BitsInUnignedInt =
            (unsigned int)(sizeof(unsigned int) * 8);
        unsigned int ThreeQuarters = (unsigned int)((BitsInUnignedInt * 3) / 4);
        unsigned int OneEighth = (unsigned int)(BitsInUnignedInt / 8);
        unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt

                                                               - OneEighth);
        unsigned int hash = 0;
        unsigned int test = 0;

        while (*str) {
            hash = (hash << OneEighth) + (*str++);
            if ((test = hash & HighBits) != 0) {
                hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
            }
        }

        return (hash & 0x7FFFFFFF);
    }

    // ELF Hash
    unsigned int ELFHash(char* str) {
        unsigned int hash = 0;
        unsigned int x = 0;

        while (*str) {
            hash = (hash << 4) + (*str++);
            if ((x = hash & 0xF0000000L) != 0) {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }

        return (hash & 0x7FFFFFFF);
    }

    unsigned int BKDRHash1(char* str) {
        unsigned int seed = 1313; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        while (*str) {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    // DJB Hash
    unsigned int DJBHash(char* str) {
        unsigned int hash = 5381;

        while (*str) {
            hash += (hash << 5) + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    // AP Hash
    unsigned int APHash(char* str) {
        unsigned int hash = 0;
        int i;

        for (i = 0; *str; i++) {
            if ((i & 1) == 0) {
                hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
            } else {
                hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
            }
        }

        return (hash & 0x7FFFFFFF);
    }
};

// 常用字符串哈希函数有BKDRHash，APHash，DJBHash，JSHash，RSHash，SDBMHash，PJWHash，ELFHash等等。
// SDBMHash
unsigned int SDBMHash(char* str) {
    unsigned int hash = 0;

    while (*str) {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash
unsigned int RSHash(char* str) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash
unsigned int JSHash(char* str) {
    unsigned int hash = 1315423911;

    while (*str) {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash
unsigned int PJWHash(char* str) {
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters = (unsigned int)((BitsInUnignedInt * 3) / 4);
    unsigned int OneEighth = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt

                                                           - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;

    while (*str) {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash
unsigned int ELFHash(char* str) {
    unsigned int hash = 0;
    unsigned int x = 0;

    while (*str) {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash
unsigned int BKDRHash(char* str) {
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

unsigned int BKDRHash1(char* str) {
    unsigned int seed = 1313; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash
unsigned int DJBHash(char* str) {
    unsigned int hash = 5381;

    while (*str) {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// AP Hash
unsigned int APHash(char* str) {
    unsigned int hash = 0;
    int i;

    for (i = 0; *str; i++) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
}