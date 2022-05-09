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
    nonedt,
    void,
    integer,
    float,
    string,
    function,
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
    int id;    //唯一id标识
    Type type; //节点类型
    DataType dataType;   //节点数据类型
    int child_num;
    std::vector<AST *> *children;
    
    baseAST(Type type, const char *name);
    ~baseAST();
    void Insert(AST *);
    void print(void);
};
class varNode:public baseAST{ /* only for var */
public:
    Type type=T_var;
    std::string name;
};
class constNode:public baseAST{
    Type type=T_const;
    Value dvalue; /* only for const */
}
class operatorNode:public baseAST{
    Type type=T_operator;
    Operator op;  /* only for operator */
}

class 
#endif