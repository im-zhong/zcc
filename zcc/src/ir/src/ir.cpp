// 2023/2/26
// zhangzhong
// ir

#include "ir/ir.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace ir {

auto make_empty_symbol_list() -> SymbolList { return {}; }
// todo
InstructionPtr make_binary_assignment(int op, std::string left, int left_type,
                                      std::string right, int right_type,
                                      std::string result) {
    return std::make_shared<Instruction>(Instruction{.op = op,
                                                     .left = left,
                                                     .left_type = left_type,
                                                     .right = right,
                                                     .right_type = right_type,
                                                     .result = result});
}

InstructionPtr make_assignment(std::string left, int left_type,
                               std::string result) {
    return std::make_shared<Instruction>(Instruction{.op = IR::ASSIGN,
                                                     .left = left,
                                                     .left_type = left_type,
                                                     .result = result});
}

CodeList make_empty_code_list() { return {}; }

LabelPtr make_label(std::string label) {
    return std::make_shared<Label>(Label{.label = label});
}

BranchPtr make_full_branch(std::string symbol, int type, std::string true_label,
                           std::string false_label) {
    return std::make_shared<Branch>(Branch{
        .condition =
            TypedSymbol{
                .symbol = symbol,
                .type = type,
            },
        .true_label = true_label,
        .false_label = false_label,
    });
}

BranchPtr make_half_branch(std::string symbol, int type,
                           std::string true_label) {
    return std::make_shared<Branch>(Branch{
        .condition =
            TypedSymbol{
                .symbol = symbol,
                .type = type,
            },
        .true_label = true_label,
    });
}

BranchPtr make_goto(std::string label) {
    return std::make_shared<Branch>(Branch{
        .true_label = label,
    });
}

FnCallPtr make_fncall(std::string name, SymbolList parameter_list) {
    return std::make_shared<FnCall>(FnCall{
        .name = name,
        .parameter_list = parameter_list,
    });
}

FnCallPtr make_fncall_assignment(std::string name, SymbolList parameter_list,
                                 std::string result, int return_type) {
    return std::make_shared<FnCall>(FnCall{
        .name = name,
        .result = result,
        .return_type = return_type,
        .parameter_list = parameter_list,
    });
}

std::string type_to_string(int type) {
    switch (type) {
    case IR::BOOL:
        return "bool";
    case IR::I8:
        return "i8";
    case IR::I16:
        return "i16";
    case IR::I32:
        return "i32";
    case IR::I64:
        return "i64";
    case IR::F32:
        return "f32";
    case IR::F64:
        return "f64";
    default:
        exit(EXIT_FAILURE);
    }
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

std::string Instruction::to_string() const {
    // 现在我们需要检查它是不是assigment
    std::stringstream ss;
    if (op == IR::ASSIGN) {
        ss << result << " = " << left << ":" << type_to_string(left_type);
    } else {
        ss << result << " = " << left << ":" << type_to_string(left_type) << " "
           << op_to_string(op) << " " << right << ":"
           << type_to_string(right_type);
    }
    return ss.str();
}

std::string Label::to_string() const {
    std::stringstream ss;
    ss << label << ":";
    return ss.str();
}

std::string Branch::to_string() const {
    std::stringstream ss;
    if (condition) {
        ss << "if " << condition->symbol << ":"
           << type_to_string(condition->type) << " then " << true_label;
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
        ss << *result << " = ";
    }
    ss << "call " << name << "(";
    // 这中间是参数列表
    for (size_t i = 0; i < parameter_list.size(); i++) {
        ss << parameter_list[i].symbol << ":"
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

FnDeclPtr make_fndecl(std::string name, SymbolList parameter_list,
                      int return_type, CodeList body) {
    return std::make_shared<FnDecl>(FnDecl{
        .name = name,
        .parameter_list = parameter_list,
        .return_type = return_type,
        .body = body,
    });
}

std::string symbol_list_to_string(const SymbolList& symbol_list) {
    std::stringstream ss;
    for (size_t i = 0; i < symbol_list.size(); i++) {
        ss << symbol_list[i].symbol << ":"
           << type_to_string(symbol_list[i].type);
        if (i < symbol_list.size() - 1) {
            ss << ", ";
        }
    }
    return ss.str();
}

FnDeclPtr make_noret_fndecl(std::string name, SymbolList parameter_list,
                            CodeList body) {
    return std::make_shared<FnDecl>(FnDecl{
        .name = name,
        .parameter_list = parameter_list,
        .body = body,
    });
}

std::string FnDecl::to_string() const {
    std::stringstream ss;
    ss << "fn " << name << "(" << symbol_list_to_string(parameter_list) << ")";
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

DeclList make_empty_decl_list() { return {}; }

} // namespace ir