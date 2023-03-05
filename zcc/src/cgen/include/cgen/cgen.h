// 2023/3/4
// zhangzhong

#pragma once

#include "ir/driver.h"
#include "ir/ir.h"
#include <list>
#include <map>
#include <ranges>
#include <unordered_map>
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

    void add_fn(std::string name, InstructionList body) {}

  private:
    std::string filename;
};
// 我们需要一个condition_table
struct Condition {
    // 其实一个int就可以表示 不过这是优化阶段的事情
    // < <= > >= == !=
    int op;
    // signed unsigned
    bool is_signed;

    std::string to_string() const;
};

// 每次生成其实都是以一个函数为单位的
// 我们可以设想一下我们需要生成一个新的汇编文件
class X86CodeGenerator {
    friend struct CodeVisitor;
    friend struct DeclVisitor;

  public:
    // cgen的输入是一个ir程序文件
    // 输出一个assembly汇编文件代码 以及一个符号表
    AssemblyFile cgen(std::string filename);

    // std::string emit(ir::Decl decl);
    // std::string emit(ir::Code code);
    // 这样的话 这个函数就用来生成整个的assemblyfile对象
    // 或者也可以不是 因为我们还有一些辅助用的符号表
    // void emit_asm();
    // just for test
    InstructionList emit_asm(ir::InstructionPtr instruction);
    // InstructionList emit_cond(ir::InstructionPtr cond);
    // InstructionList emit_branch(ir::BranchPtr branch);
    // InstructionList emit_fndef(ir::FnDefPtr fndef);

  private:
    // 接下来就是实现这四个函数
    void def_fn(ir::FnDefPtr def);
    void def_symbol(ir::SymbolDefPtr def);
    void decl_struct(ir::StructDeclPtr decl);
    void decl_symbol(ir::SymbolDeclPtr decl);

    // 然后是针对所有ir语句的汇编指令生成
    InstructionList emit_expr(ir::InstructionPtr expr);
    InstructionList emit_memop(ir::InstructionPtr load_store);
    InstructionList emit_cond(ir::InstructionPtr cond);
    InstructionList emit_label(ir::LabelPtr label);
    InstructionList emit_branch(ir::BranchPtr branch);
    InstructionList emit_fncall(ir::FnCallPtr fncall);
    InstructionList emit_ret(ir::ReturnPtr ret);
    InstructionList emit_symdef(ir::SymbolDefPtr symdef);
    InstructionList emit_cast(ir::CastPtr cast);
    InstructionList emit_gep(ir::GepPtr gep);

    // void emit_decl(ir::Decl decl);
    // void emit_code(ir::Code code);

    Operand make_operand(ir::Symbol symbol);

    // 对于每一种decl单独进行分析
    void decl_global(ir::SymbolDeclPtr symbol_decl);

    void emit_code(ir::InstructionPtr instruction);

    Instruction map_code(ir::InstructionPtr instruction);

    std::string get_register_name(int id);

  private:
    std::string filename; // 记录正在处理的文件
    std::unordered_map<std::string, Symbol> symbol_table;
    std::unordered_map<std::string, Condition> condition_table;
    std::unordered_map<std::string, Symbol> global_table;
    // 我们确实需要一个type table
    // todo: Type类型需要自己实现
    std::unordered_map<std::string, ir::Type> type_table;

    // 解析的过程其实就是填充这个汇编文件
    AssemblyFile asm_file;
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

struct CodeVisitor {
    // todo 这些函数体都太长了 应该简化到只有一行代码
    void operator()(ir::InstructionPtr expr) {
        // 然后它的返回值要怎么返回呢
        // 然后这里调用我们为每个ir类型写好的实现函数
        // 然后返回值
        // list.merge(cgen.emit_cond(expr));

        for (auto inst : cgen.emit_cond(expr)) {
            list.push_back(inst);
        }
        // todo
        cgen.emit_expr(expr);
        cgen.emit_memop(expr);
    }
    void operator()(ir::LabelPtr label) {
        for (auto inst : cgen.emit_label(label)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::BranchPtr branch) {
        for (auto inst : cgen.emit_branch(branch)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::FnCallPtr fncall) {
        for (auto inst : cgen.emit_fncall(fncall)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::ReturnPtr ret) {
        for (auto inst : cgen.emit_ret(ret)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::SymbolDefPtr symdef) {
        // 这个会生成代码吗?? 不对 会的 会生成一个栈寄存器的减法
        for (auto inst : cgen.emit_symdef(symdef)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::CastPtr cast) {
        for (auto inst : cgen.emit_cast(cast)) {
            list.push_back(inst);
        }
    }
    void operator()(ir::GepPtr gep) {
        for (auto inst : cgen.emit_gep(gep)) {
            list.push_back(inst);
        }
    }

    X86CodeGenerator& cgen;
    InstructionList& list;
};

struct DeclVisitor {
    void operator()(ir::StructDeclPtr decl) { cgen.decl_struct(decl); }
    void operator()(ir::FnDefPtr def) { cgen.def_fn(def); }
    void operator()(ir::SymbolDefPtr def) { cgen.def_symbol(def); }
    void operator()(ir::SymbolDeclPtr decl) { cgen.decl_symbol(decl); }
    X86CodeGenerator& cgen;
};

// 对于碰到的cond ir 我们需要分析

} // namespace cgen