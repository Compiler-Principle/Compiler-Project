#ifndef AST_H
#define AST_H
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <cstring>
#include <fstream>
#include "../info/info.h"
#include "json.hpp"
using json_t = nlohmann::json;

typedef enum AST_Type
{
    T_root, // 0
    T_none, // 1
    T_operator, // 2
    T_var, //3
    T_fvar, //4
    T_const, //5
    T_func, //6
    T_defi, //7
    T_list, //8
    T_tydf,  //9
    T_expr, //10
    T_formatstr, //11
    T_block, //12
} Type;

typedef enum AST_DataType
{
    DT_nonedt,
    DT_void,
    DT_integer,
    DT_float,
    DT_string,
    DT_function,
} DataType;

typedef union AST_Value
{
    int integer;
    char *str;
    float floatt;
} Value;

typedef enum AST_Operator
{
    O_AND,
    O_OR,
    O_ADD,
    O_MINUS,
    O_MULT,
    O_DIV,
    O_EQUAL,
    O_LE,
    O_GE,
    O_NE,
    O_LESS,
    O_GREATER,
    O_LOGICAND,
    O_MINUSDIGIT,
    O_NOT,
    O_noneop,
    // O_OR,
    // O_AND,
    // O_OP,
    // O_ADD,
    // O_EQ,
    // O_LE,
    // O_GE,
    // O_NE,
    // O_LT,
    // O_GT,
    // O_add,
    // O_min,
    // O_mul,
    // O_ddi,
    // O_mod,
} Operator;

class Var {
public:
    std::string name;
    AST_DataType type;
    Var(std::string name, AST_DataType type) : name(std::move(name)), type(type) {}

    ~Var() {}

};


class Func {
public:
    std::string name;
    AST_DataType type;
    std::map<std::string, Var *> localVars; // local variables in the specific function

    Func(std::string name, AST_DataType rtype): name(std::move(name)), type(rtype) {}

    ~Func(){}
};


class baseAST{
public:

    static int IDAccumulate;
    unsigned id;    //唯一id标识
    std::string name; // 名称，用于存储和传递
    AST_Type type; //节点类型
    AST_DataType dataType;   //节点数据类型
    unsigned childCnt;
    std::vector<baseAST *> children;

    baseAST();
    baseAST(AST_Type type, AST_DataType dataType);
    baseAST(AST_Type type, std::string name);
    // ~baseAST();
    void Insert(baseAST *);
    void print(void);
    void buildTable(Func *scope);
};

class varNode : public baseAST{
public:
    std::string name; /* only for var */
    varNode(const char *name, AST_DataType dataType);
    ~varNode();
};


class constNode : public baseAST{
public:
    AST_Value dvalue; /* only for const */
    constNode(int value, AST_DataType dataType);
    constNode(char *value, AST_DataType dataType);
    constNode(float value,AST_DataType dataType);
    ~constNode();
};

class operatorNode : public baseAST{
public:

    AST_Operator op;  /* only for operator */
    operatorNode(AST_Operator op, AST_DataType dataType);
    operatorNode(AST_Operator op, std::string name);
    ~operatorNode();
};


#endif