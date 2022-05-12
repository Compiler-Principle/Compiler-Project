#ifndef AST_H
#define AST_H
#include <utility>
#include <vector>
#include <string>
#include <map>
#include "../info/info.h"

typedef enum Type
{
    T_none,
    T_operator,
    T_var,
    T_const,
    T_func,
} Type;

typedef enum DataType
{
    DT_nonedt,
    DT_void,
    DT_integer,
    DT_float,
    DT_string,
    DT_function,
} DataType;

typedef union Value
{
    int integer;
    char *str;
} Value;

typedef enum Operator
{
    O_noneop,
    O_OP,
    O_AD,
    O_EQ,
    O_LE,
    O_GE,
    O_NE,
    O_LT,
    O_GT,
    O_add,
    O_min,
    O_mul,
    O_ddi,
    O_mod,
} Operator;


class Var {
public:
    std::string name;
    DataType type;
    Var(std::string name, DataType type) : name(std::move(name)), type(type) {}

    ~Var() {}

};


class Func {
public:
    std::string name;
    DataType type;
    std::map<std::string, Var *> localVars; // local variables in the specific function

    Func(std::string name, DataType rtype): name(std::move(name)), type(rtype) {}

    ~Func(){}
};


class baseAST{
public:

    static int IDAccumulate;
    unsigned id;    //唯一id标识
    std::string name; // 名称，用于存储和传递
    Type type; //节点类型
    DataType dataType;   //节点数据类型
    unsigned childCnt;
    std::vector<baseAST *> children;

    baseAST();
    baseAST(Type type,DataType dataType);
    baseAST(Type type, std::string name);
    // ~baseAST();
    void Insert(baseAST *);
    void print(void);
    void buildTable(Func *scope);
};

class varNode : public baseAST{
public:
    std::string name; /* only for var */
    varNode(const char *name,DataType dataType);
    ~varNode();
};

class constNode : public baseAST{
public:
    Value dvalue; /* only for const */
    constNode(int value,DataType dataType);
    constNode(char *value,DataType dataType);
    ~constNode();
};

class operatorNode : public baseAST{
public:
    Operator op;  /* only for operator */
    operatorNode(Operator op,DataType dataType);
    ~operatorNode();
};


#endif