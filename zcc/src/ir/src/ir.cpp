// 2023/2/26
// zhangzhong
// ir

#include "ir/ir.h"
#include "ir/type.h"
#include <cassert>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace ir {

auto make_instruction(Instruction instruction) -> InstructionPtr {
    return std::make_shared<Instruction>(instruction);
}
auto make_label(Label label) -> LabelPtr {
    return std::make_shared<Label>(label);
}
auto make_branch(Branch branch) -> BranchPtr {
    return std::make_shared<Branch>(branch);
}
auto make_fncall(FnCall fncall) -> FnCallPtr {
    return std::make_shared<FnCall>(fncall);
}
auto make_return(Return ret) -> ReturnPtr {
    return std::make_shared<Return>(ret);
}
auto make_symbol_def(SymbolDef symbol_def) -> SymbolDefPtr {
    return std::make_shared<SymbolDef>(symbol_def);
}
auto make_fndef(FnDef fndef) -> FnDefPtr {
    return std::make_shared<FnDef>(fndef);
}
auto make_struct_decl(StructDecl struct_decl) -> StructDeclPtr {
    return std::make_shared<StructDecl>(struct_decl);
}
auto make_cast(Cast cast) -> CastPtr { return std::make_shared<Cast>(cast); }
auto make_symbol_decl(SymbolDecl symbol_decl) -> SymbolDeclPtr {
    return std::make_shared<SymbolDecl>(symbol_decl);
}
auto make_gep(Gep gep) -> GepPtr { return std::make_shared<Gep>(gep); }

auto make_empty_symbol_list() -> SymbolList { return {}; }
auto make_empty_code_list() -> CodeList { return {}; }
auto make_empty_decl_list() -> DeclList { return {}; }

std::string type_to_string(Type type) {
    std::stringstream ss;
    std::visit([&ss](auto type) { ss << ir::to_string(*type); }, type);
    return ss.str();
}

std::string op_to_string(int op) {
    switch (op) {
    case IR::ADD:
        return "+";
    case IR::SUB:
        return "-";
    case IR::MUL:
        return "*";
    case IR::DIV:
        return "/";
    case IR::MOD:
        return "%";
    case IR::BITAND:
        return "&";
    case IR::BITOR:
        return "|";
    case IR::BITXOR:
        return "^";
    case IR::LSH:
        return "<<";
    case IR::RSH:
        return ">>";
    case IR::LESS:
        return "<";
    case IR::LESS_QEUAL:
        return "<=";
    case IR::GREAT:
        return ">";
    case IR::GREAT_EQUAL:
        return ">=";
    case IR::EQUAL:
        return "==";
    case IR::NOT_EQUAL:
        return "!=";
    default:
        exit(EXIT_FAILURE);
    }
}

std::string Label::to_string() const {
    std::stringstream ss;
    ss << name << ":";
    return ss.str();
}

std::string Instruction::to_string() const {
    // 现在我们需要检查它是不是assigment
    // std::stringstream ss;
    // if (op == IR::ASSIGN) {
    //     // ss << result << " = " << left << ":" << type_to_string(left_type);
    //     return fmt::format("{} = {}", ir::to_string(result),
    //                        ir::to_string(left));
    // } else if (op == IR::ADDROF) {
    //     return fmt::format("{} = addrof {}", ir::to_string(result),
    //                        ir::to_string(left));
    // } else if (op == IR::LOAD) {
    //     return fmt::format("{} = load {}", ir::to_string(result),
    //                        ir::to_string(left));
    // } else if (op == IR::STORE) {
    //     return fmt::format("store {} to {}", ir::to_string(left),
    //                        ir::to_string(result));
    // } else {
    //     // ss << result << " = " << left << ":" << type_to_string(left_type)
    //     <<
    //     // " "
    //     //    << op_to_string(op) << " " << right << ":"
    //     //    << type_to_string(right_type);
    //     return fmt::format("{} = {} {} {}", ir::to_string(result),
    //                        ir::to_string(left), op_to_string(op),
    //                        ir::to_string(*right));
    // }
    // return ss.str();

    if (right) {
        return fmt::format("{} = {} {} {}", ir::to_string(result),
                           ir::to_string(left), op_to_string(op),
                           ir::to_string(*right));
    } else {
        if (op == IR::ASSIGN) {
            // ss << result << " = " << left << ":" <<
            // type_to_string(left_type);
            return fmt::format("{} = {}", ir::to_string(result),
                               ir::to_string(left));
        } else if (op == IR::ADDROF) {
            return fmt::format("{} = addrof {}", ir::to_string(result),
                               ir::to_string(left));
        } else if (op == IR::LOAD) {
            return fmt::format("{} = load {}", ir::to_string(result),
                               ir::to_string(left));
        } else if (op == IR::STORE) {
            return fmt::format("store {} to {}", ir::to_string(left),
                               ir::to_string(result));
        }
    }
    return "error";
}

std::string Branch::to_string() const {
    std::stringstream ss;
    if (condition) {
        ss << "if " << ir::to_string(*condition) << " then " << true_label;
        if (false_label) {
            ss << " else " << *false_label;
        }
    } else {
        // goto
        ss << "goto " << true_label;
    }
    return ss.str();
}

std::string FnCall::to_string() const {
    std::stringstream ss;
    if (result) {
        ss << ir::to_string(*result) << " = ";
    }
    ss << "call " << name << "(";
    // 这中间是参数列表
    for (size_t i = 0; i < parameter_list.size(); i++) {
        ss << parameter_list[i].name << ":"
           << type_to_string(parameter_list[i].type);
        if (i < parameter_list.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    if (return_type) {
        ss << " -> " << type_to_string(*return_type);
    }
    return ss.str();
}

std::string symbol_list_to_string(const SymbolList& symbol_list) {
    std::stringstream ss;
    for (size_t i = 0; i < symbol_list.size(); i++) {
        ss << symbol_list[i].name << ":" << type_to_string(symbol_list[i].type);
        if (i < symbol_list.size() - 1) {
            ss << ", ";
        }
    }
    return ss.str();
}

std::string FnDef::to_string() const {
    std::stringstream ss;
    ss << "fn " << name << "(" << symbol_list_to_string(parameter_list);
    if (varargs) {
        ss << ", ...";
    }
    ss << ")";
    // 检查是否拥有返回值
    if (return_type) {
        ss << " -> " << type_to_string(*return_type);
    }
    ss << " {\n";

    // 输出函数体
    for (const auto& v : body) {
        // visit就是遍历variant的值
        // 这里的auto推导的类型是 shared_ptr
        // 然后to_string是一个模板函数 会调用类型的to_string
        std::visit(
            [&ss](auto code) {
                ss << "    " << ir::to_string(*code) << std::endl;
            },
            v);
    }

    ss << "}";
    return ss.str();
}

std::string Return::to_string() const {
    std::stringstream ss;
    if (return_value) {
        ss << "ret " << return_value->name << ":"
           << type_to_string(return_value->type);
    } else {
        ss << "ret none";
    }
    return ss.str();
}

std::string StructDecl::to_string() const {
    std::stringstream ss;
    ss << "struct " << name << " {\n";
    for (size_t i = 0; i < fields.size(); i++) {
        ss << "    " << type_to_string(fields[i]);
        if (i < fields.size() - 1) {
            ss << ",\n";
        }
    }
    ss << "\n}";
    return ss.str();
}

std::string SymbolDef::to_string() const {
    // return name + " = local " + type_to_string(type);
    std::string position = local ? "local" : "global";
    if (size) {
        return fmt::format("{} = {} {}, {}", ir::to_string(result), position,
                           type_to_string(type), ir::to_string(*size));
    }
    return fmt::format("{} = {} {}", ir::to_string(result), position,
                       type_to_string(type));
}

std::string cast_to_string(int cast) {
    switch (cast) {
    case IR::BITCAST:
        return "bitcast";
    case IR::ZEROEXT:
        return "zeroext";
    case IR::SIGNEXT:
        return "signext";
    case IR::TRUNC:
        return "trunc";
    case IR::FEXT:
        return "fext";
    case IR::FTRUNC:
        return "ftrunc";
    default:
        assert(false);
    }
}

std::string Cast::to_string() const {
    // return result + " = " + cast_to_string(cast) + " " + value + ":" +
    //        type_to_string(value_type) + " to " +
    //        type_to_string(result_type);
    return fmt::format("{} = {} {} to {}", ir::to_string(result),
                       cast_to_string(cast), ir::to_string(value),
                       type_to_string(type));
}

std::string Symbol::to_string() const {
    return fmt::format("{}:{}", name, type_to_string(type));
}

std::string SymbolDecl::to_string() const {
    return fmt::format("decl {}", ir::to_string(symbol));
}

std::string Gep::to_string() const {
    return fmt::format("{} = gep {}, {}, {}", ir::to_string(result),
                       ir::to_string(value), ir::to_string(array_index),
                       ir::to_string(struct_index));
}

bool Symbol::is_global() const { return name[0] == '@'; }
bool Symbol::is_local() const { return name[0] == '%'; }
bool Symbol::is_constant() const { return name[0] == '$'; }
std::string Symbol::trim() const { return name.substr(1); }

bool is_cond(int op) {
    return op == IR::LESS || op == IR::LESS_QEUAL || op == IR::GREAT ||
           op == IR::GREAT_EQUAL || op == IR::EQUAL || op == IR::NOT_EQUAL;
}

} // namespace ir
