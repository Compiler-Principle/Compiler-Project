#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
#include <map>

typedef enum Type
{
    none,
    defi,
    func,
    cconst,
    var,
    fvar,
    list,
    opr,
    tydf,
    expr,
    lvalue
} Type;

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

typedef enum DataType
{
    nonedt,
    vvoid,
    integer,
    string,
    function,
} DataType;

typedef union Value
{
    int integer;
    char *str;
} Value;

class Var_attr;
class Fun_attr;
class AST;
typedef std::map<std::string, Var_attr *> Vmap;
typedef std::map<std::string, Fun_attr *> Fmap;
extern AST *head;
extern Vmap glovars;
extern Fmap funs;

class Var_attr
{
public:
    std::string name;
    int dim;
    DataType dtype;
    Fun_attr *belong;
    std::vector<int> *dimention;

    Var_attr(std::string name, DataType dtype)
    {
        this->dim = 0;
        this->name = name;
        this->dtype = dtype;
        this->belong = NULL;
        this->dimention = NULL;
    }
    ~Var_attr() { delete this->dimention; }
};

class Fun_attr
{
public:
    std::string name;
    int argc;
    DataType rtype;
    Fun_attr *belong;
    Vmap *locvars;
    Fmap *locfuns;
    std::vector<std::pair<DataType, std::string>> *argv;
    std::vector<std::pair<std::string, Var_attr *>> *parents_argv;

    Fun_attr(std::string name, DataType rtype)
    {
        this->name = name;
        this->argc = 0;
        this->rtype = rtype;
        this->belong = NULL;
        this->locvars = new Vmap;
        this->locfuns = new Fmap;
        this->argv = NULL;
        this->parents_argv = NULL;
    }
    ~Fun_attr()
    {
        delete this->locvars;
        delete this->locfuns;
        delete this->argv;
        delete this->parents_argv;
    }
};

class AST
{
public:
    static int IDAccumulate;
    int id;
    Type ntype;
    std::string name; /* only for var */
    DataType dtype;
    Value dvalue; /* only for const */
    Operator op;  /* only for operator */
    int child_num;
    std::vector<AST *> *children;

    AST(Type type, const char *name);
    AST(int value);
    AST(char *value);
    ~AST();
    void Insert(AST *);
    void BuildTable(Fun_attr *);
    bool CheckTable(Fun_attr *);
    void print(void);
};

#endif