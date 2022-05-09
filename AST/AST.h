#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
#include <map>
typedef enum Type
{
    operator,
    var,
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

class baseAST{
public:
    int id;    //唯一id标识
    Type type; //节点类型
    DataType dataType;   //节点数据类型
    int child_num;
    std::vector<AST *> *children;
    
    AST(Type type, const char *name);
    ~AST();
    void Insert(AST *);
    void print(void);
};
class 

#endif