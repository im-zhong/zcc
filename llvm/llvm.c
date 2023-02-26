// 2023/2/24
// zhangzhong

// int fun() {
//     // 只要是声明的变量就一定会放在栈上
//     // 这样简单呀
//     int i = 1;
//     int j = 2;
//     int k = i + j;

//     for (int i = 0; i < 10; i++) {
//         k += i;
//     }

//     return k;
// }

int test_logical() {
    int i = 1;
    int j = 2;
    if (i && j) {
        j = 3;
    }
    return j;
}

int test_pointer() {
    //   %1 = alloca i32, align 4
    //   store i32 1, ptr %1, align 4
    int i = 1;
    //   %2 = alloca ptr, align 8
    //   store ptr %1, ptr %2, align 8
    int* pi = &i;
    //   %3 = alloca i32, align 4
    //   %4 = load ptr, ptr %2, align 8
    //   %5 = load i32, ptr %4, align 4
    //   store i32 %5, ptr %3, align 4
    int j = *pi;
    //   %6 = load i32, ptr %3, align 4
    //   ret i32 %6
    return j;
}

double test_double_pointer() {
    double i = 1.0;
    double* pi = &i;
    double j = *pi;
    return j;
}

struct student {
    int id;
    char* name;
};

struct student* test_struct_pointer() {
    struct student* stu;
    return stu;
}

int test_ssa_phi() {
    int x = 1;
    int flag = 2;
    if (flag) {
        x = -1;
    } else {
        x = 3;
    }
    int y = x;
    return y;
}

// llvm 怎么处理 unsigned ??
// 所以在llvm眼里 数字就是一个固定bit的数字而已
// 因为在cpu内部并不会区分有符号数和无符号数
// 他们的唯一需要注意的地方就是采取 符号扩展还是无符号扩展而已
int test_unsigned() {
    // 那如果我给一个超过int的整数呢
    int i = 1;
    unsigned char j = 1;
    // %6 = zext i8 %5 to i32
    int k = i + j;
    //%11 = sext i8 %10 to i32
    char l = 1;
    k = i + l;
    return k;
}

// llvm怎么处理global
// global变量最终会保存在静态区 所以其值必须在编译期可知
// 所以最终结果会以一个常数的形式保留下来
const int i = 1;
int j = i * 8;
// 默认值也会给零
int k;

int test_minus() {
    int i = 1;
    // %4 = sub nsw i32 0, %3
    int j = -i;
    // %7 = xor i32 %6, -1
    int k = ~j;
    return k;
}

int test_mod() {
    int i = 1;
    int j = 2;
    // remainder mod
    //  %6 = srem i32 %4, %5
    int k = i % j;
    return k;
}