#pragma once
#ifndef _ZCC_STRUCT_TYPE_H_
#define _ZCC_STRUCT_TYPE_H_

#include "aggregate_type.h"
#include "primitive_type.h"
#include "symbol/symbol.h"
#include "type_metric.h"
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

_CML_BEGIN_

// 结构体只能被定义一次 但是可以被多次声明
// struct Point
// {int x; int y;}
// 这就是要给定义 只能出现一次
// struct Point; 这只是一个声明

class StructType : public AggregateType {
  public:
    // 这个是无域
    // 那么也不一定对应这一个声明 可能是没有没有域的定义
    // 也可能是之后会添加定义
    // 唯一会发生冲突的情况就是 这次构造是一个定义 is_define = true
    // 并且 符号表中的也存在同名且以定义的结构
    StructType(const std::string& struct_name, bool is_define)
        : AggregateType(struct_mask, struct_name) {
        // 如果已经被定义 且还想再次定义 那么就会出错
        // 否则就是一次定义
        // 有两种情况 以声明但未定义
        // 未声明
        // 但是有可能此时符号表中已经存在了声明
        check_definition(struct_name);
    }

    StructType(const std::string& struct_name,
               const std::list<std::shared_ptr<Symbol>>& field_symbols,
               bool is_define)
        : AggregateType(struct_mask, struct_name) {
        // 同上
        // 不对 所有的aggregate_type都需要查询是否存在同名以定义类型
        // 设置为AggregateType的一个函数就可以了
        // 我不太确定这个函数后有没有正确的行为
        check_definition(struct_name);
        insert(field_symbols);
    }

    virtual bool
    insert(const std::list<std::shared_ptr<Symbol>>& field_symbols) override {
        for (const auto& symbol : field_symbols) {
            if (!this->insert(field_symbols))
                return false;
        }
        return true;
    }

    virtual bool insert(const std::shared_ptr<Symbol>& symbol) override {
        // 一开始先查找同名符号 如果有同名符号 直接报错
        if (find(symbol->name) == nullptr) {
            // 一开始 offset = size
            // insert
            // 插入结束之后 size += offset
            fields.emplace_back(make_field(symbol, size));
            size += symbol->get_size();
            align = size;
            return true;
        } else
            return false;
    }

    virtual std::string to_string() const noexcept override;

  protected:
    // TODO 等符号表实现出来再完成
    // 检查该类型是否已经声明或定义 如果是返回类型
    // 如果不是返回nullptr
    // 根据返回的值可以判断是未声明 nullptr
    // 声明未定义 non-nullptr no_defined
    // 声明且定义  non-nullptr defined
    bool check_definition(const std::string& name) const noexcept override {
        // find
        // auto type = check_definition("name"); // 这里其实就是一个Find动作啊
        // if (type)
        //{
        //	// 以存在
        //	if (type->is_defined()) // 已定义
        //	{
        //		if (is_define) // 重复定义 报错
        //			throw std::runtime_error("struct " + name + "
        // 重复定义"); 		else
        //		{
        //			// 已经定义但这次只是声明 那么就没事
        //		}
        //	}
        //}
        // else
        //{
        //	// 未声明
        //	if (is_define)
        //	{
        //		// 添加定义
        //	}
        //	else
        //	{
        //		// 添加声明
        //	}
        //	defined = is_define;
        //}
        return false;
    }

  private:
};

// 又名结构可能是声明
std::shared_ptr<StructType>
make_struct(const std::string& struct_name,
            const std::list<std::shared_ptr<Symbol>>& field_symbols,
            bool is_define);
std::shared_ptr<StructType> make_struct(const std::string& struct_name,
                                        bool is_define);

// 无名结构肯定是定义
std::shared_ptr<StructType>
make_struct(const std::list<std::shared_ptr<Symbol>>& field_symbols);
std::shared_ptr<StructType> make_struct();
_CML_END_
#endif // _ZCC_STRUCT_TYPE_H_