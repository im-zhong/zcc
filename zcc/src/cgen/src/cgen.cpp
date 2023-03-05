// 2023/3/4
// zhangzhong

#include "cgen/cgen.h"
#include "cgen/x86.h"
#include "ir/driver.h"
#include "ir/ir.h"
#include "ir/type.h"
#include <array>
#include <fmt/core.h>
#include <limits>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>

// todo: 2023/3/5
//  1. IR需要区分load和store 因为只有这两个指令需要和内存打交道
//  2. IR需要区分三种symbol的类型 虽然现在可以通过前缀进行区分
//  3. 修改ir::Instruction的名字 改成Expression

// input

namespace cgen {

std::unique_ptr<X86CodeGenerator> make_x86_cgen() {
    return std::make_unique<X86CodeGenerator>();
}

// 这个函数几乎就是这个样子了 所有的处理都在内部进行
// 这个AssemblyFile应该在内部生成 而不是外部
AssemblyFile X86CodeGenerator::cgen(std::string filename) {
    this->filename = filename;
    auto driver = ir::driver{};
    driver.parse(filename);

    // step1: 首先分析decl list 然后填充global符号表
    for (const auto& decl : driver.get_decl_list()) {
        std::visit(DeclVisitor{*this}, decl);
    }

    // step2: 挨个分析每个函数 给每个函数的内部生成一个小的符号表
    // 用来描述内部符号引用的offset等

    return {};
}

void X86CodeGenerator::decl_global(ir::SymbolDeclPtr symbol_decl) {
    // 这个就是定义了一个符号 但是我们也不知道他是外部的还是内部的
    // 除非我们碰到了他的定义 symdef 否则我们都认为他是外部的
    Symbol symbol;
    symbol.name = symbol_decl->symbol.name;
    // symbol还可以包含一个type
    // 那我们只需要区分这个符号是一个变量还是一个函数即可
    // 函数指针是函数还是变量??
    // symbol.type = symbol_decl->symbol.type;
    auto type = symbol_decl->symbol.type;
    bool is_function = false;
    std::visit(ir::call_is_function{.result = is_function}, type);
    if (is_function) {
        // 1 代表函数
        symbol.type = 1;
    } else {
        // 2 代表ojbect
        symbol.type = 0;
    }

    // symbol的实现不太好
    // variant不如想象中好用 variant很难提供类似OO特性的支持
    // 不能无脑的用
    // symbol.size = type->get_size();

    // todo
    // 不行 用不了 必须是类型本身才能用
    // 而且相比OO用起来也太麻烦了吧
    // type->get_size();
    // 返回值在什么地方兄弟

    std::visit(ir::call_get_size{.size = symbol.size}, type);
    // std::visit([](ir::typable auto type) { return type.get_size(); }, *type);
    // std::variant再研究研究

    // 然后我需要把这个符号加入到符号表中
    symbol_table.insert({symbol.name, symbol});
}

void X86CodeGenerator::emit_code(ir::InstructionPtr instruction) {
    // 不对 我这里应该做一个映射
}

// InstructionList X86CodeGenerator::map_code(ir::InstructionPtr instruction) {
//     Instruction inst;

//     // 首先先生成一个move指令
//     auto mov = make_mov();
//     auto inst = make_inst();
//     // 然后这两个指令组成一个list 一起返回
//     return {mov, inst};

//     // // 像 r1 = r2 + r3
//     // // 这样的代码需要生成两句指令
//     // inst.op = mapping.at(instruction->op);
//     // // inst.type = instruction.left.type;

//     // return {};
// }

// mov只支持寄存器和常量
// 我们让参数的顺序和汇编的指令顺序一致
Instruction make_mov(int type, Operand src, Operand dst) { return {}; }

Instruction make_instruction(int op, int type, Operand src, Operand dst) {
    return Instruction{
        .op = op,
        .type = type,
        .src = src,
        .dst = dst,
    };
}

std::string op_to_string(int op) { return inst_to_string(op); }
std::string type_to_string(int type) {

    // 这个简单
    switch (type) {
    case X86::BYTE:
        return "b";
    case X86::WORD:
        return "w";
    case X86::DWORD:
        return "l";
    case X86::QWORD:
        return "q";
    case X86::FLOAT:
        return "s";
    case X86::DOUBLE:
        return "l";
    default:
        std::cout << "type to string bug" << std::endl;
        return "";
    };
}

std::string Instruction::emit() const {
    // 一般而言 指令格式都是
    // 某个指令可能是有label的
    // label:
    //      op[postfix] operand, operand
    // 提供两个函数
    // 1. op(int) -> op(string)
    // 2. type(int) -> type(string)
    // 然后这两个名字合成一个后缀

    std::stringstream ss;
    if (label) {
        ss << *label << ":\n";
    }

    // 然后合成指令名字
    ss << op_to_string(op) << type_to_string(type) << " ";
    // 然后给出操作数
    // 那么不同的操作数有不同的输出方式
    // 这个地方可以使用variant

    // 这个也分指令 有的是没有两个操作数的额
    // 其实只有特殊的指令没有两个操作数
    // 或者说 我们可以使用optional 这样更加简单
    std::visit([&ss](const auto& operand) { ss << operand.to_string(); }, src);
    if (dst) {
        std::visit(
            [&ss](const auto& operand) { ss << ", " << operand.to_string(); },
            *dst);
    }

    return ss.str();
}

// 然后给所有的operand实现to_string即可

std::array<std::string, 1> register_file = {
    "ax",
};

std::string get_register_name(int id) { return std::to_string(id); }

std::string Immediate::to_string() const { return imm; }
std::string Register::to_string() const { return reg; }
std::string Label::to_string() const { return name; }
std::string Memory::to_string() const {
    // 这个就需要根据寻址模式来了
    switch (mode) {
    case X86::ABSOLUTE:
        return displacement->to_string();
    case X86::INDIRECT:
        return fmt::format("({})", base->to_string());
    case X86::BASE_DISPLACEMENT:
        return fmt::format("{}({})", displacement->to_string(),
                           base->to_string());
    case X86::INDEXED:
        return fmt::format("({}, {})", base->to_string(), index->to_string());
    case X86::INDEXED_DISPLACEMENT:
        return fmt::format("{}({}, {})", displacement->to_string(),
                           base->to_string(), index->to_string());
    default:
        // bug
        return "memory to string bug";
    }
}

std::string X86CodeGenerator::emit(ir::Decl decl) { return "todo"; }
std::string X86CodeGenerator::emit(ir::Code code) { return "todo"; }

int to_x86_type(ir::Type type) {
    // 如果是basic type那么就转换成byte
    // 如果是其他的类型 直接转换指针类型 Quard Word 即可
    using ir::IR;
    if (auto t = std::get_if<ir::BasicTypePtr>(&type); t) {
        switch ((*t)->type) {
        case IR::BOOL:
            return X86::BYTE;
        case IR::I8:
            return X86::BYTE;
        case IR::I16:
            return X86::WORD;
        case IR::I32:
            return X86::DWORD;
        case IR::I64:
            return X86::QWORD;
        case IR::F32:
            return X86::FLOAT;
        case IR::F64:
            return X86::DOUBLE;
        default:
            printf("to x86 type bug\n");
        };
    }
    return X86::QWORD;
}

// 不行啊 我们需要根据他的op和type共同选择指令
int to_x86_inst(int op, int type) {
    // 这个就不太好 其实可以搞一个映射数组的
    // 没有必要用switch
    static std::array<int, 32> mapping = {
        X86::MOV, X86::LEA, X86::MOV, X86::MOV, X86::ADD, X86::SUB, X86::IMUL,
    };

    using ir::IR;
    return mapping[op - IR::ASSIGN];
}

Operand X86CodeGenerator::make_operand(ir::Symbol symbol) {
    // 一共有三种类型的operand
    // 1. 立即数
    if (symbol.is_constant()) {
        return Immediate{.imm = "$" + symbol.trim()};
    }

    if (symbol.is_local()) {
        // 这就意味着这是一个由ir生成的寄存器
        // 名字最好还是不要丢了
        return Register{.reg = "%" + symbol.trim()};
    }

    if (symbol.is_global()) {
        // 那么这里的操作数应该是一个符号
        return Label{.name = symbol.trim()};
    }

    // todo: 那么我们怎么表示内存操作数呢
    // 先不急 这个跑通了再说
    return {};
}

InstructionList X86CodeGenerator::emit_asm(ir::InstructionPtr expr) {
    // ir::Expression -> cgen::Instruction
    // result = left op right
    // mov left, result
    // op right, result

    // mov src, dst
    // op src, dst

    // 1. 第一步是生成mov
    Instruction mov;
    mov.op = X86::MOV;
    // 不对啊 我们要给出type啊
    // 也就是byte word dword ... 这些东西呀
    // 单纯的给出size没有用 无法确定后缀
    // 反而是给出了
    mov.type = to_x86_type(expr->result.type);
    // 我们需要根据指令的符号进行operand的构造
    mov.src = make_operand(expr->left);
    mov.dst = make_operand(expr->result);

    // 第二步 生成op
    Instruction inst;
    inst.op = to_x86_inst(expr->op, mov.type);
    inst.type = mov.type;
    if (expr->right) {
        inst.src = make_operand(*expr->right);
    }
    inst.dst = make_operand(expr->result);

    return {mov, inst};
}

void X86CodeGenerator::emit_decl(ir::Decl decl) {}

// 所以Cond需要一个新的类型
InstructionList X86CodeGenerator::emit_cond(ir::InstructionPtr cond) {
    // 首先 我们需要判断传入的指令是否是cond指令
    assert(ir::is_cond(cond->op));

    // 那么必然是有左右符号的
    // 那么我们就要生成cmp指令
    Instruction cmp;
    cmp.op = X86::CMP;
    cmp.type = to_x86_type(cond->left.type);
    // cmp的顺序和ir的顺序相反
    cmp.src = make_operand(*cond->right);
    cmp.dst = make_operand(cond->left);

    // 那么现在我们需要填充一个cond table
    // 卧槽 有缺陷 数据类型没有无符号数
    // todo: IR的基础类型需要添加无符号数
    // 我真是吐了，variant真是太难用啦！！！
    auto cond_symbol = Condition{.op = cond->op, .is_signed = true};
    // 插入condition table
    condition_table[cond->result.name] = cond_symbol;

    return {cmp};
}

InstructionList X86CodeGenerator::emit_branch(ir::BranchPtr branch) {
    // todo
    return {};
}

InstructionList X86CodeGenerator::emit_fndef(ir::FnDefPtr fndef) {
    // name 那么这个名字需要添加到全局符号表中

    // 其实可以最后再添加 因为size我们也不知道
    // 函数的size是用一条特殊的句子完成的
    // 就是在函数的最后一条句子里面加上 .size fnname, .-fnname
    global_table[fndef->name] = Symbol{.name = fndef->name, .type = 1};

    // parameter list的作用是告诉我们参数在哪里 如果函数内部的某个指令引用了
    // 参数 那么我们必须为其生成正确的地址
    // 额 可变参数的参数个数怎么来的呢?? 竟然没有 惊了
    // 那就这样吧

    // 卧槽 实现一个这样的类不就行了吗!!!!!!!
    // 把相应的代码生成都放在这里面
    InstructionList list;
    for (const auto& code : fndef->body) {
        // 这个code是一个variant
        // 针对这个variant中的每一个类型，我们都需要进行特殊的处理
        // 卧槽 突然觉得非常合适
        std::visit(CodeVisitor{*this, list}, code);
    }

    return list;
}

void X86CodeGenerator::emit_asm() {}

} // namespace cgen
