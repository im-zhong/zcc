
#include "ast/parser.h"
#include "ast/declaration_sematic.h"
#include "ast/syntax_error.h"
#include "core/grammar_abstract.h"
#include "type/type.h"

using namespace std;

_CML_BEGIN_

// 暂时作为测试函数
void Parser::do_parser() {
    // Token int_token = { TERMINAL::RES_INT,"int" };
    // Token i_token = { TERMINAL::ID,"i" };
    // Token fenhao = { TERMINAL::DLM_SEMIC,";" };

    // token_vector.push_back(int_token);
    // token_vector.push_back(i_token);
    // token_vector.push_back(fenhao);

    cout << "开始语法分析" << endl;

    // 我也可以在一开始做一些检测 来快速判断错误
    // TODO C标准规定每个声明至少要声明一个表示符 一个结构或枚举标记
    // 或者一个或多个枚举成员
    // 声明需要在分配去分配空间
    token = next_token();
    // 我们可以首先判断token是否为空 如果为空 直接报错就可以了
    translation_unit();

    cout << "translation unit 分析完毕" << endl;
}

bool Parser::is_external_declaration(const Token& token) {
    return is_declaration_specifiers(token);
}

bool Parser::is_storage_class_specifier(const Token& token) {
    return storage_class_specifier_table.find(token.lexeme) !=
           storage_class_specifier_table.end();
}

bool Parser::is_type_specifier(const Token& token) {
    return type_specifier_table.find(token.lexeme) !=
           type_specifier_table.end();
    // 单纯的ID是不对的 因为这里其实希望的是一个类型
    // 所以这里是一个typename才对
    // || token.category == TERMINAL::ID;
}

bool Parser::is_type_qualifier(const Token& token) {
    return type_qualifier_table.find(token.lexeme) !=
           type_qualifier_table.end();
}

bool Parser::is_init_declarator(const Token& token) {
    return token.lexeme == "(" || token.lexeme == "*" ||
           token.category == TERMINAL::ID;
}

// 暂时不实现typedef
bool Parser::is_declaration_specifiers(const Token& token) {
    return token.lexeme == "(" || token.lexeme == "*" ||
           is_storage_class_specifier(token) || is_type_specifier(token) ||
           is_type_qualifier(token);
}

bool Parser::is_fields(const Token& token) {
    return is_type_specifier(token) || is_type_qualifier(token) ||
           is_declarator(token);
}

bool Parser::is_declarator(const Token& token) {
    return token.lexeme == "*" || token.lexeme == "(" ||
           token.category == TERMINAL::ID;
}

bool Parser::is_abstract_declarator(const Token& token) {
    return is_declarator(token);
}

void Parser::init() {
    type_specifier_table.insert({"void", "char", "float", "short", "signed",
                                 "int", "double", "long", "unsigned", "struct",
                                 "union", "enum"});

    type_qualifier_table.insert({"const", "volatile"});

    storage_class_specifier_table.insert(
        {"typedef", "extern", "static", "auto", "register"});
}

/*
translation-unit :
        external-declaration { external-declaration }
*/
void Parser::translation_unit() {
    cout << "translation unit" << endl;
    external_declaration();
    while (is_external_declaration(token)) {
        external_declaration();
    }
}

/*
external-declaration :
        function-definition
        declaration
*/
void Parser::external_declaration() {
    cout << "external declaration" << endl;

    // 假设没有函数定义
    declaration();
}

/*
function-definition :
        declaration-specifiers declarator compound-statement
*/
void Parser::function_definition() { cout << "function definition" << endl; }

/*
declaration	:
        declaration-specifiers [ init-declarator { ',' init-declarator} ] ';'

declaration :
        declaration-specifiers  ';'
        declaration-specifiers init-declarator { ',' init-declarator}  ';'

*/
/*
        语义的特殊处理
        我在这里的实现与书中不同
        因为函数定义只能在文件作用域
        所以如果你检查到这是一个函数定义 只需要检查现在是什么作用域就可以了
        不需要区分什么第一次declarator第二次declaratory 这种诡异的设计

        declaration的语义相当于声明在声明符中给出的标识符
        int i = 0;
        int由declaration specifiers给出
        i = 0;由init delcaration 给出
        declaration的工作就是把这个两者结合起来 定义存储空间 声明变量
   添加符号表之类的 定义一个DeclarationSemantic来解决这个问题

*/
void Parser::declaration() {
    cout << "declaration" << endl;

    // declaration-specifiers
    declaration_specifiers();
    // [ init-declarator { ',' init-declarator} ]
    if (is_init_declarator(token)) {
        init_declarator();
        while (token.lexeme == ",") {
            token = next_token();
            init_declarator();
        }
    }

    if (token.lexeme == ";") {
        token = next_token();
    } else {
        syntax_error("声明应该以分号结束");
    }
}

/*
init-declarator	:
        declarator [ '=' initializer ]
*/
void Parser::init_declarator() {
    cout << "init declarator" << endl;

    // declarator
    declarator();
    // [ '=' initializer ]
    if (token.lexeme == "=") {
        token = next_token();
        initializer();
    }
}

/*
initializer :
        assignment-expression
        '{' initializer { ',' initializer } [ ',' ] '}'
*/
void Parser::initializer() {
    // '{' initializer { ',' initializer } [ ',' ] '}'
    if (token.lexeme == "{") {
        token = next_token();
        // initializer
        initializer();
        // { ',' initializer }
        while (token.lexeme == ",") {
            token = next_token();
            initializer();
        }
        // [ ',' ]
        if (token.lexeme == ",")
            next_token();
        // '}'
        if (token.lexeme == "}") {
            next_token();
        } else {
            syntax_error("初始化列表应该以大括号结束");
        }
    } else {
        // assignment-expression
        // 那么我们假设不存在
        syntax_error("目前还没有实现初始化声明");
    }
}

/*
declarator :
        pointer direct-declarator { suffix-declarator }
        direct-declarator { suffix-declarator }
*/
/*

*/
void Parser::declarator() {
    cout << "declarator" << endl;

    // pointer
    pointer();
    // direct-declarator
    direct_declarator();
    // { suffix-declarator }
    while (token.lexeme == "(" || token.lexeme == "[") {
        suffix_declarator();
    }
}

/*
pointer :
        { '*' { type-qualifier } }

        指针语义
        * cosnt volatile * const volatile * ...

*/
void Parser::pointer() {
    cout << "pointer" << endl;

    std::stack<std::pair<bool, bool>> reverse_type;
    std::pair<bool, bool> pointer_qualifier;

    // { '*' { type-qualifier } }
    while (token.lexeme == "*") {
        pointer_qualifier.first = false;  // const
        pointer_qualifier.second = false; // volatile
        token = next_token();

        // 在一个*后面最多跟一个const 和一个 volatile 跟多了是错的
        // { type-qualifier }
        while (is_type_qualifier(token)) {
            // type_qualifier();
            if (token.lexeme == "const") {
                if (!pointer_qualifier.first) {
                    pointer_qualifier.first = true;
                    token = next_token();
                } else {
                    syntax_error("错误的指针声明");
                }
            } else if (token.lexeme == "volatile") {
                if (!pointer_qualifier.second) {
                    pointer_qualifier.second = true;
                    token = next_token();
                } else {
                    syntax_error("错误的指针声明");
                }
            }
        }

        // 这就声明了一层指针
        reverse_type.push(pointer_qualifier);
    }

    // 在最后我们可以返回正序的指针
    // int * cosnt * volatile * const volatile * p;
    // 也就是按照声明顺序返回指针
}

/*
direct-declarator :
        identifier
        '(' declarator ')'
*/
void Parser::direct_declarator() {
    cout << "direct declarator" << endl;

    // '(' declarator ')'
    if (token.lexeme == "(") {
        token = next_token();
        declarator();
        if (token.lexeme == ")") {
            token = next_token();
        } else {
            syntax_error("缺少)");
        }
    }
    // identifier
    else if (token.category == TERMINAL::ID) {
        token = next_token();
    } else {
        syntax_error("direct declarator syntax error.");
    }
}

/*
suffix-declarator :
        '[' [ constant-expression ] ']'
        '(' [ parameter-list ] ')'

        语义计算
        这里要处理两个语义 也就是数组和函数的声明
*/
void Parser::suffix_declarator() {
    cout << "sufix declarator" << endl;
    DeclQualifierType type(int_type);
    // 不对 这里写多了
    // '[' [ constant-expression ] ']'
    if (token.lexeme == "[") {
        // 数组后缀语义计算
        // 需要传入基础类型

        array_suffix(type);
    }
    // '(' [ parameter-list ] ')'
    else if (token.lexeme == "(") {
        // 这里传入的实际上是函数的返回类型把
        function_suffix(type);
    } else {
        syntax_error("suffix declarator syntax error");
    }
}

// 这个函数应该可以获得一个类型
// '[' [ constant-expression ] ']'
void Parser::array_suffix(DeclQualifierType type) {
    token = next_token();
    // 检查是否是常量表达式
    int array_size = 0;
    // array_size constant expression
    if (token.lexeme == "]") {
        token = next_token();
        type.type = make_array(type.type, array_size);
    } else {
        syntax_error("缺少]");
    }
}

// 函数参数列表
void Parser::function_suffix(DeclQualifierType type) {
    token = next_token();
    // 减产是否为参数列表或者为空
    // 能不能断定这里声明的是一个函数呢？
    // is_declaration_specifiers
    if (is_declaration_specifiers(token)) {
        parameter_list();
    }
    if (token.lexeme == ")") {
        token = next_token();
    } else {
        syntax_error("函数声明缺少)");
    }
}

/*
parameter-list :
        parameter { ',' parameter } [ ',' "..." ]
*/
void Parser::parameter_list() {
    cout << "parameter list" << endl;
    // 一旦开始读取参数列表中的内容 我们就需要打开一个新的作用域
    enter_scope();

    // 从参数列表可以获取返回类型
    std::list<shared_ptr<Type>> parameter_type_list;

    // parameter
    // 收集parameter返回的类型信息或者符号信息 保存成函数
    parameter();
    // { ',' parameter } [ ',' "..." ]
    while (token.lexeme == ",") {
        token = next_token();
        if (is_declaration_specifiers(token)) {
            parameter();
        } else if (token.lexeme == "...") {
            // 可变参数
            // 函数声明必须在此结束
            token = next_token();
            break;
            // 我可以选择直接跳出 或者设置一个标志进行检测 报告这种错误
        } else {
            // 报告错误的地方要不要继续读取直到一个可以停止的位置呢？
            syntax_error("parameter list syntax error");
        }
    }

    // 最后在这里可以组成一个函数类型
    // 参数列表读取完毕我们需要离开这个作用域
    exit_scope();
}

/*
parameter :
        declaration-specifiers ( declarator | [ abstract-declarator ] )

declarator :
        pointer direct-declarator { suffix-declarator }

abstract-declarator :
        '*' { type-qualifier }
        pointer '(' abstract-declarator ')'
        { suffix-abstract-declarator }
        pointer { suffix-abstract-declarator }


        出大问题 这两个产生式的first集是一样的啊
parameter :
        declaration-specifiers
follow(parameter) declaration-specifiers  declarator
first(declarator) declaration-specifiers  abstract-declarator
first(abstract-declarator)
*/
void Parser::parameter() {
    cout << "parameter" << endl;

    // declaration-specifiers
    declaration_specifiers();
    // TOOD 3/24 这里这样实现就不能实现读取不带参数名的函数声明了
    if (is_declarator(token)) {
        declarator();
    } else if (token.lexeme == "*" || token.lexeme == "[" ||
               token.lexeme == "(") {
        // 第二个产生式是可以推空的
        // 所以仅使用first集是不行的还要看 parameter的follow集
        // 不对 如果产生式推空 那么就意味着我们做了一次无用的调用
        // 所以是first还是follow其实是调用了不同的产生
        // 对于推空的产生的调用应该放弃
        abstract_declarator();
    }
    // TODO  到头来我还是需要自己写一份first集
    else if (token.lexeme == ")" ||
             token.lexeme ==
                 ",") // follow 集 那么我们应该直接放行 因为这些符号不在这里处理
    {
        // 但是实际上不受影响啊
        // 在这里就算是follow我也不需要做任何事情 因为调用parameter的
        // 函数会帮我兜底
    } else {
        // 对于其他情况我们应该报错
        // 这个报错就算不在这里报错 也会在递归结束之后报错
        syntax_error("parameter syntax error");
    }
}

/*
declaration-specifiers :
        storage-class-specifier [ declaration-specifiers ]
        type-sepcifier [ declaration-specifiers ]
        type-qualifier [ declaration-specifiers ]
*/
// 在调用本函数时需要在外部进行检查 否则可能会出错
/*
        TOOD 语义分析
                short const x;
                const short x;
                const short int x;
                int const short x;
                这些声明都是一样的 但是顺序不一样 所以实现起来非常复杂
                storage-class specifier type-specifier type-qualifier
                可以以任意顺序出现 但是一种说明符只能出现一次
                type-specifier应该是必须出现一次

                typdef 的名字会作为ID符号处理，他只能有一个存储类型或限定符
                C++好像不能指定存储类型啊
                typedef type identifier;
*/
DeclQualifierType Parser::declaration_specifiers() {
    cout << "declaration specifiers" << endl;

    // storage-class-specifier
    // 3/24 旧的实现
    // 分析：因为本函数递归调用了自身 所以直接改成死循环就行了
    // storage-class-specifier [ declaration-specifiers ]
    // if (is_storage_class_specifier(token))
    //{
    //	storage_class_specifier();
    //	if (is_declaration_specifiers(token))
    //	{
    //		declaration_specifiers();
    //	}
    //}
    //// type-sepcifier [ declaration-specifiers ]
    // else if (is_type_specifier(token))
    //{
    //	type_specifier();
    //	if (is_declaration_specifiers(token))
    //	{
    //		declaration_specifiers();
    //	}
    // }
    //// type-qualifier [ declaration-specifiers ]
    // else if (is_type_qualifier(token))
    //{
    //	type_qualifier();
    //	if (is_declaration_specifiers(token))
    //	{
    //		declaration_specifiers();
    //	}
    // }

    // for (;;)
    //{
    //	if (is_storage_class_specifier(token))
    //	{
    //		if (!is_storage_class)
    //		{
    //			is_storage_class = true;
    //			storage_class = token.category;
    //			storage_class_specifier();
    //		}
    //		else
    //		{
    //			syntax_error("不能重复声明存储类型");
    //		}
    //	}
    //	// type-sepcifier [ declaration-specifiers ]
    //	else if (is_type_specifier(token))
    //	{
    //		// 这里要同时考虑 type sign
    //		// short int
    //		// long int
    //		// long
    //		// int long
    //		// signed short int
    //		// long char error!
    //		if (!is_type)
    //		{
    //			type_specifier();
    //		}
    //

    //	}
    //
    //	// type-qualifier [ declaration-specifiers ]
    //	else if (is_type_qualifier(token))
    //		type_qualifier();
    //}

    // 3/24 带语义的实现
    // uint8_t storage_class = TERMINAL::RES_AUTO;
    // bool is_storage_class = false;
    //// type-qualifier
    // bool is_const = false;
    // bool is_volatile = false;
    //// type-specifier
    // bool is_sign;
    // uint8_t type;
    // bool is_type = false;
    // int size = 0; // long short
    // shared_ptr<Type> declaration_type = nullptr;
    auto semantic = make_declaration_specifier_semantic();
    bool declaration_specifier_done = false;
    // 首先使用这个循环收集类型信息
    // 也就是上面这些变量 一个一个的填充起来
    // TODO 我可以声明一个变量保存每次的semantic.set的结果 在最后检查就可以了啊
    // 。。。。
    for (;;) {
        switch (token.category) {
        case TERMINAL::RES_AUTO:;
        case TERMINAL::RES_REGISTER:
            if (scope <= 0 && !semantic.set_storage_class(token.category))
                syntax_error("全局声明的存储类型不能为auto&"
                             "register或者是重复的存储类型声明");
            else
                token = next_token();
            break;

        case TERMINAL::RES_STATIC:;
        case TERMINAL::RES_EXTERN:;
        case TERMINAL::RES_TYPEDEF:
            if (semantic.set_storage_class(token.category))
                token = next_token();
            else
                syntax_error("重复的存储类型声明");
            break;

        case TERMINAL::RES_CONST:
            if (semantic.set_const_qualifier(true))
                token = next_token();
            else
                syntax_error("重复的const声明");
            break;
        case TERMINAL::RES_VOLATILE:
            if (semantic.set_volatile_qualifier(true))
                token = next_token();
            else
                syntax_error("重复的volatile声明");
            break;

        case TERMINAL::RES_SIGNED:;
        case TERMINAL::RES_UNSIGNED:
            if (semantic.set_sign(
                    token.category == TERMINAL::RES_SIGNED ? true : false))
                token = next_token();
            else
                syntax_error("重复的符号声明");
            break;

        case TERMINAL::RES_LONG:;
        case TERMINAL::RES_SHORT:
            if (semantic.set_size(token.category))
                token = next_token();
            else
                syntax_error("重复的long short声明");
            break;

        case TERMINAL::RES_VOID:;
        case TERMINAL::RES_CHAR:;
        case TERMINAL::RES_INT:;
        case TERMINAL::RES_FLOAT:;
        case TERMINAL::RES_DOUBLE:
            if (semantic.set_type(token.category))
                token = next_token();
            else
                syntax_error("重复的类型声明");
            break;

        case TERMINAL::RES_ENUM:
            if (semantic.set_type(token.category)) {
                // 在这里直接调用枚举的类型声明
                enum_specifier();
                declaration_specifier_done = true;
            } else
                syntax_error("重复的类型声明");
            break;

        case TERMINAL::RES_STRUCT:
        case TERMINAL::RES_UNION:
            if (semantic.set_type(token.category)) {
                // 直接调用结构体或枚举的类型声明
                struct_or_union_specifier();
                declaration_specifier_done = true;
            } else
                syntax_error("重复的类型声明");
            break;
        case TERMINAL::ID:
            // 这里这么写是不对的
            // 因为 int i;
            // 这样的声明语句i不应该被处理 所以当我们读到ID/
            // 但是检查之后发现ID不是typename应该直接跳出循环 结束
            if (semantic.set_type(semantic.is_typename(token.lexeme))) {
                token = next_token();
            } else {
                declaration_specifier_done = true;
            }
            break;
        default:
            declaration_specifier_done = true;
            break;
        }

        if (declaration_specifier_done)
            break;
    }

    // 然后根据上面这些变量进行语义分析
    return semantic.compute();
}

/*
storage-class-sepcifier :
        "typdef" | "extern" | "static" | "auto" | "register"
*/
void Parser::storage_class_specifier() {
    cout << "storage class specifier" << endl;
    token = next_token();
}

/*
type-specifier :
        "void"
        "char" | "float" | "short" | "signed"
        "int" | "double" | "long" | "unsigned"
        struct-or-union-specifier
        enum-specifier
        typedef-name
*/
void Parser::type_specifier() {
    cout << "type specifier" << endl;
    if (token.lexeme == "struct" || token.lexeme == "union") {
        struct_or_union_specifier();
    } else if (token.lexeme == "enum") {
        enum_specifier();
    }
    // typedef-name
    else if (token.category == TERMINAL::ID) {
        typedef_name();
    }
    // void char ...
    else {
        token = next_token();
    }
}

void Parser::typedef_name() {
    cout << "typedef name" << endl;

    // 当前的token是一个typedef
    // 我需要实现一个功能 就是像类型表中添加一个类型
    // 或者从类型表中寻找一个类型
    // TypeTable type_table;
    // type_table.find(token.lexeme);
    // type_table.insert({ "cint",Type(const int) });

    token = next_token();
    syntax_error("没有实装符号表你怎么使用自定义类型啊！");
}

/*
type-qualifier :
        "const" | "volatile"
*/
void Parser::type_qualifier() {
    cout << "type qualifier" << endl;
    token = next_token();
}

/*
struct-or-union-specifier :
        struct-or-union [ identifier ] '{' fields { fields } '}'
        struct-or-union identifier
*/
void Parser::struct_or_union_specifier() {
    cout << "struct or union specifier" << endl;

    // struct-or-union
    struct_or_union();

    bool has_name = false;
    bool has_fields = false;

    // identifier
    if (token.category == TERMINAL::ID) {
        has_name = true;
        token = next_token();
    }
    // '{' fields { fields } '}'
    if (token.lexeme == "{") {
        has_fields = true;
        token = next_token();
        //  fields
        fields();
        // { fields }
        while (is_fields(token)) {
            fields();
        }
        // '}'
        if (token.lexeme == "}") {
            token = next_token();
        } else {
            syntax_error("struct or union specifier error");
        }
    }
    // 如果又没名 又没域 报错
    if (!has_name && !has_fields)
        syntax_error("结构或联合必须有名字");
}

void Parser::struct_or_union() {
    cout << "struct or union" << endl;
    token = next_token();
}

/*
fields :
        { type-specifier | type-qualifier } field { ',' field } ';'
*/
void Parser::fields() {
    cout << "fields" << endl;

    // { type-specifier | type-qualifier }
    for (;;) {
        if (is_type_specifier(token)) {
            type_specifier();
        } else if (is_type_qualifier(token)) {
            type_qualifier();
        } else
            break;
    }
    // field
    field();
    // { ',' field }
    if (token.lexeme == ",") {
        token = next_token();
        field();
    }
    // ';'
    if (token.lexeme == ";") {
        token = next_token();
    } else {
        syntax_error("结构或联合声明必须以分号结束");
    }
}

/*
field :
        declarator
*/
void Parser::field() {
    cout << "field" << endl;
    declarator();
}

/*
enum-specifier :
        "enum" [ identifier ] '{' enumerator { ',' enumerator } '}'  //
为什么枚举的声明不是以分号结束的 "enum" identifier
*/
void Parser::enum_specifier() {
    cout << "enum specifier" << endl;
    bool has_name = false;
    bool has_fields = false;
    if (token.lexeme == "enum") {
        token = next_token();
        // identifier
        if (token.category == TERMINAL::ID) {
            token = next_token();
            has_name = true;
        }
        // '{' enumerator { ',' enumerator } '}'
        if (token.lexeme == "{") {
            token = next_token();
            has_fields = true;
            // enumerator
            enumerator();
            // { ',' enumerator }
            while (token.lexeme == ",") {
                token = next_token();
                enumerator();
            }
            // '}'
            if (token.lexeme == "}") {
                token = next_token();
            } else {
                syntax_error("联合域声明必须以大括号结束");
            }
        }
        // 如果无名无域报错
        if (!has_name && !has_fields) {
            syntax_error("只写一个enum是错的");
        }
    } else {
        syntax_error("枚举关键字enum错误");
    }
}

/*
enumerator :
        identifier [ '=' constant-expression ]
*/
void Parser::enumerator() {
    cout << "enumerator" << endl;
    // identifier
    if (token.category == TERMINAL::ID) {
        token = next_token();
        // [ '=' constant-expression ]
        if (token.lexeme == "=") {
            token = next_token();
            // constant_expression
            token = next_token(); // 假设这是一个常量表达式
            syntax_error("枚举常量初始化还未实现");
        }
    } else {
        syntax_error("枚举常量必须是一个标识符");
    }
}

/*
个人认为这个句子不对 typename怎么可以推空呢？
type-name :
        { type-specifier | type-qualifier } [ abstract-declarator ]
修改之后的产生式
type-name : (type-specifier | type-qualifier){ type-specifier | type-qualifier
}[ abstract-declarator ]
*/
void Parser::type_name() {
    cout << "type name" << endl;
    // (type-specifier | type-qualifier)
    if (is_type_specifier(token)) {
        type_specifier();
    } else if (is_type_qualifier(token)) {
        type_specifier();
    } else {
        syntax_error("错误的类型名");
    }
    // { type-specifier | type-qualifier }
    for (;;) {
        if (is_type_specifier(token)) {
            type_specifier();
        } else if (is_type_qualifier(token)) {
            type_specifier();
        } else
            break;
    }
    // [ abstract-declarator ]
    if (is_abstract_declarator(token)) {
        abstract_declarator();
    }
}

/*


注意这里的pointer不能推空
abstract_declarator : pointer
                        | pointer direct_abstract_declarator
                        |   direct_abstract_declarator
                        ;

direct_abstract_declarator : '(' abstract_declarator ')'
                        | direct_abstract_declarator '[' const_exp ']'
                        |               '[' const_exp ']'
                        | direct_abstract_declarator '['    ']'
                        |               '[' ']'
                        | direct_abstract_declarator '(' param_type_list ')'
                        |               '(' param_type_list ')'
                        | direct_abstract_declarator '('        ')'
                        |               '('     ')'
                        ;

abstract-declarator :
        '*' { type-qualifier }
        pointer '(' abstract-declarator ')'
        { suffix-abstract-declarator }
        pointer { suffix-abstract-declarator }
*/
void Parser::abstract_declarator() {
    cout << "abstract declarator" << endl;

    // 实际上这里一共有三个进入点 * ( [
    // pointer
    // pointer direct_abstract_declarator
    if (token.lexeme == "*") {
        pointer();
        // 可以明显看出上面的产生式在
        // pointer '(' abstract-declarator ')'
        // pointer { suffix-abstract-declarator }
        // 存在冲突 因为 "("既可以选择第一条 也可以选择第二条
        // 这个时候需要向前查看一下
        if (token.lexeme == "[" || token.lexeme == "(") {
            suffix_abstract_declarator();
            while (token.lexeme == "[" || token.lexeme == "(")
                suffix_abstract_declarator();
        } else {
            syntax_error(
                "我发现在递归的底层处理错误是很自然的，虽然这样是最慢的");
        }
    } else if (token.lexeme == "[" || token.lexeme == "(") {
        suffix_abstract_declarator();
        while (token.lexeme == "[" || token.lexeme == "(")
            suffix_abstract_declarator();
    } else {
        syntax_error("我发现在递归的底层处理错误是很自然的，虽然这样是最慢的");
    }
}

/*
suffix-abstract-declarator :
        '[' [ constant-expression ] ']'
        '(' [ parameter-list ] ')'
*/
void Parser::suffix_abstract_declarator() {
    cout << "suffix abstract declarator" << endl;
    // 这两个函数的分析是一样的
    // suffix_declarator();

    // 只能通过重写这个函数来实现这个功能了
    cout << "sufix declarator" << endl;

    // 不对 这里写多了
    // '[' [ constant-expression ] ']'
    if (token.lexeme == "[") {
        token = next_token();
        // 检查是否是常量表达式
        syntax_error("没有实现常量表达式");
        if (token.lexeme == "]") {
            token = next_token();
        } else {
            syntax_error("缺少]");
        }
    }
    // '(' [ parameter-list ] ')'
    // '(' abstract_declarator ')'
    else if (token.lexeme == "(") {
        token = next_token();
        // 减产是否为参数列表或者为空
        // 能不能断定这里声明的是一个函数呢？
        // [ parameter-list ]
        if (is_declaration_specifiers(token)) {
            parameter_list();
        }
        // '(' abstract_declarator ')'
        else if (token.lexeme == "*" || token.lexeme == "[" ||
                 token.lexeme == "(") {
            abstract_declarator();
        } else {
            syntax_error("错误的声明");
        }

        if (token.lexeme == ")") {
            token = next_token();
        } else {
            syntax_error("函数声明缺少)");
        }
    } else {
        syntax_error("suffix declarator syntax error");
    }
}

_CML_END_