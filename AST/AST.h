#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
#include <map>
typedef enum Type
{
    T_operator,
    T_var,
    T_const,
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
    noneop,
    OR,
    AD,
    EQ,
    LE,
    GE,
    NE,
    LT,
    GT,
    add,
    min,
    mul,
    ddi,
    mod,
} Operator;

class baseAST{
public:
    unsigned id;    //唯一id标识
    Type type; //节点类型
    DataType dataType;   //节点数据类型
    unsigned childCnt;
    std::vector<baseAST *> children;

    baseAST();
    baseAST(Type type, const char *name);
    ~baseAST();
    void Insert(AST *);
    void print(void);
};
class varNode : public baseAST{
public:
    std::string name; /* only for var */
    varNode(const char *name);
};

class constNode : public baseAST{
public:
    Value dvalue; /* only for const */
    constNode(Value value);
};

class operatorNode : public baseAST{
public:
    Operator op;  /* only for operator */
    operatorNode(Operator op);
};


#endif