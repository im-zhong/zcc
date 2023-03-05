// 2023/3/4
// zhangzhong

#pragma once

#include "ir/driver.h"
#include "ir/ir.h"
#include <ranges>
#include <variant>

namespace cgen {

// 对于每一种ir我们都需要提供一个实现
// InstructionPtr emit(ir::InstructionPtr instruction);
// InstructionPtr emit(ir::Label label);
// InstructionPtr emit(ir::Branch branch);
// InstructionPtr emit(ir::FnCall fncall);

int new_register();

// register
// 怎么表示
struct Immediate {
    std::string imm;

    std::string to_string() const;
};
struct Register {
    // 其实这个id应该是递增的
    std::string reg;

    std::string to_string() const;
};
struct Memory {
    // 不行啊 这个寻址模式很难通过判断给了那个值做出选择
    // 还是一个枚举来得实在
    int mode;
    std::optional<Register> base;
    std::optional<Register> index;
    std::optional<Immediate> displacement;

    std::string to_string() const;
};
struct Label {
    std::string name;

    std::string to_string() const;
};

// 现在这个symbol可以是很多东西
// 立即数 寄存器 mem label(name)
using Operand = std::variant<Label, Register, Memory, Immediate>;

// 然后我需要他们可以变成字符串

// dest = dest op src
struct Instruction {
    std::optional<std::string> label;
    int op;
    int type;
    Operand src;
    std::optional<Operand> dst;

    // std::string to_string() const;
    std::string emit() const;
};
using InstructionList = std::vector<Instruction>;

// 其实这个玩意的数据结构可以模仿link里面的哪个
struct Symbol {
    std::string name;
    int type;
    int size;
    bool global;
    int section;
    int offset;
};

// 那么一个函数其实就是指令的列表
// 同时函数也是代码生成的单位
struct Function {
    InstructionList body;
};

class AssemblyFile {
  public:
    // 这个东西专门用来生成汇编代码

    void emit_header();
    void emit_footer();

    // 现在一个函数由许多的指令组成
    // 我们要做的就是以函数为单位 将ir映射成汇编代码
    // 然后以函数为单位进行寄存器分配 然后组织成汇编代码文件即可
    // 在进行生成期间 我们仍然需要一个简单的符号表
    // 我们需要记录全局符号 函数内部的临时符号 常量 标号 等等

    // 输出调试信息
    std::string to_string() const;

    // 生成给定文件名的文件
    bool write(std::string filename);

  private:
    std::string filename;
};

// 每次生成其实都是以一个函数为单位的
// 我们可以设想一下我们需要生成一个新的汇编文件
class X86CodeGenerator {
  public:
    // cgen的输入是一个ir程序文件
    // 输出一个assembly汇编文件代码 以及一个符号表
    AssemblyFile cgen(std::string filename);

    std::string emit(ir::Decl decl);
    std::string emit(ir::Code code);

    InstructionList emit_asm(ir::InstructionPtr instruction);

  private:
    void emit_decl(ir::Decl decl);
    void emit_code(ir::Code code);

    Operand make_operand(ir::Symbol symbol);

    // 对于每一种decl单独进行分析
    void decl_global(ir::SymbolDeclPtr symbol_decl);

    void emit_code(ir::InstructionPtr instruction);

    Instruction map_code(ir::InstructionPtr instruction);

    std::string get_register_name(int id);

    std::string filename; // 记录正在处理的文件
    std::unordered_map<std::string, Symbol> symbol_table;
};

// interface
// class CodeGenerator {
//   public:
//     // 我们都可以进行代码的转换
//     // 也就是生成无限寄存器的x86代码
//     virtual std::string emit(ir::Decl decl) = 0;
//     virtual std::string emit(ir::Code code) = 0;
// };

std::unique_ptr<X86CodeGenerator> make_x86_cgen();

} // namespace cgen