// 2023/2/26
// zhangzhong
// ir

#include "ir/ir.h"
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>

namespace ir {
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

} // namespace ir