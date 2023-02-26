# 自己设计一套ir
# 然后根据这个ir生成代码
# 想想就刺激

# 三地址码 参考龙书和llvmir

1. local or temp variable
- %tmp12345345 不允许特殊符号 
- global @main
- 其实我们还可以用$ $tmp 这个看起来更像变量吧 只不过在汇编代码里面 %确实表示寄存器 @也确实表示全局变量 所以还是挺合理的 $表示常数 没必要麻烦

# type
1. bool, i8, i32, i64, u8, u32, u64, f32, f64
2. ptr: *bool, *i8, ...
3. type some_type = struct {...}; 
4. function: fn 
5. array: 只支持一维数组 []ptr
6. 

# 类型系统 采用golang式的系统
1. pointer to i32: *i32
2. array: [12]i32 
3. function: fn(i32, ...)void

# binary assignment
%t1 = %t2 op %t3
%1 = %2 + %3  这样读起来更简单，但是怎么标注类型呢??
%1: i32 = %2: i32 + %3: i32 这样写没有可读性 冒号标注类型不行
用.后置类型就是比较好的选择了
%1 = add.i32 %1.i32, %2.i32

1. add, sub, mul, div
2. bitand, bitor, bitxor
3. lsh, rsh

# unary assignment
%t1 = op %t2
1. minus 
2. bitnot
3. ptr 取地址 &

# load, sotre
1. %t1 = load.type ptr
2. store.type ptr, value   这个代码也特别不易读 因为你分不清顺序 只有这一条代码不是=
    ptr = store.type value 这样真的好吗??

# assignment
1. %t1 = %t2

# unconditional jmp
1. goto label

# cond, bool expression, binary assignment, 只不过结果一定是bool
1. %cond = op %1, %2
2. eq, ne, slt, sle, sgt, sge, ult, ule, ugt, uge
3. 我就问问你这有可读性？？？ 读到这样的代码不先愣住才怪了
4. 一个表达式只需要一个地方可以表述类型就行了，因为我们一定会保证所有的操作数都是同一类型的, 这就是cast的意义啊
5. %cond = %1 > %2, > >= < <= == !=
6. 像上面这样的代码是不是究极易读，其实和直接读源代码也没啥太大区别， 唯一的问题是我们需要找一个地方塞上类型注释
5. %cond.bool = %1.i32 > %2.i32

# conditional jmp
1. if <cond> then L1 else L2
2. if <cond> then L1

# function, procedure
1. %r.void = call @func(%1.i32, %2.f64)

# struct, 会产生一个新的类型
type foo = struct {
    i8,
    *f64,
    *foo
}

%1.i32 = size(type)

%1.i32 = size(i32), %1.i32 = 4

%2.*i32 = ptr(%1.i32)
store %2.*i32, 4
哦哦哦哦哦 懂了 数据从右往左流动的 方向是一致的

给每个变量都配上详细的类型标注 这就是type inference的任务
而且也可以让中间代码更加易读
%3.i32 = load %2.*i32

# declaration
1. 声明函数
decl fn @main(type1, type2, ...) -> type

2. 声明变量
decl var.type @global

# visibility, 暂时不处理
1. public
2. private

# 感觉这个东西可以用lex + yacc实现啊

# cast
1. upcast, downcast, bitcast

%1.i64 = bitcast %2.i32 to i64

2. zeroext 无符号数扩展 
3. signext 有符号数扩展

4. downcast, truncate

5. float

6. %1.i64 = ptrcast %2.*i8 to i64 


decl fn @somefn(fn(i32, f64)bool) bool
decl fn @somefn(fn(i32,f64)->bool) -> bool

%1 = call @somefn(@anotherfn.fn(i32, f64)bool) bool
%1 = call @somefn(@anotherfn.fn(i32,f64)->bool) -> bool
%1 = call @somefn(@anotherfn: fn(i32,f64)->bool, %1: f64) -> bool
第二种更好一点 带着箭头可以把复杂的部分分开 更容易阅读
采用类型注释对于函数来说确实可以提升可读性，但是对于普通的三地址代码来说 就不适用了
%1 = add.i32 %2: i32, %3: i32
%1 = %2.i32 + %3.i32
%1 = %2: i32 + %3: i32
%1 = %2:i32 + %3:i32
感觉第四种其实也可以 只要按照特定的格式生成就行了 

emmm 所以选择第三种和第四种吧

只要生成代码的时候按照特定的格式生成 阅读起来就会非常顺利
特别复杂的格式最好提供 type alias
*fn(i32,f64)->bool

type FN = fn(i32,f64)->bool

%1 = call @somefn(@anotherfn: FN) -> bool


decl global %fmt: *i8 = "hello, world\n"
fn @main(%argc: i32, %argv: **i8) {
    %1 = call @printf(%fmt: *i8) -> i32
    ret
}

# 使用flex + bison实现 读取ir文件生成ir的内存表示 indirect triple