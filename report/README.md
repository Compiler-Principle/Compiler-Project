# Compiler Principle Project
小组成员：刘舒菡，范钊瑀，季文卓
## 1 基本信息 

### 1.1 总体介绍 

我们自己设计了一个新语言并且为这个语言设计完成了一个编译器。以下是我们使用的主要工具：

+ flex
+ bison
+ LLVM
+ CMake

### 1.2 小组分工 

我们小组由三位同学组成，以下是我们的主要分工：

+ 刘舒菡：词法分析，AST设计与构建，测试用例快速排序
+ 范钊瑀：语法设计，AST可视化，符号表与函数表，测试用例选课助手，进阶主题
+ 季文卓：语法分析，中间代码生成，测试用例矩阵乘法

## 2 语法定义 fzy
### 2.1 程序结构
我们所设计的语言和c语言十分类似
整体程序由两部分构成，变量定义部分在上，函数部分在下

### 2.2 数据类型


### 2.3 基本语法规则

## 3 功能实现 

### 3.1 词法分析

我们主要采用了lex工具帮助我们来完成词法分析。将输入的文件转换为token流。具体的代码见scanner.l。

**注释**

我们的注释采用类似c语言的注释`/*注释*/`

+ 用正则表达式`/*"([^\*]|(\*)*[^\*/])*(\*)*"*/`来识别

**表达式**

我们将表达式分为以下几种类型：

+ 关系表达式：==  <=  >=  !=  <  >
+ 赋值表达式：=
+ 逻辑表达式：||  &&  !
+ 四则运算表达式：+  -  *  /

**特殊符号**

我们的语言还有以下一些特殊的符号：

+ 分号：`；`
+ 逗号：`，`
+ 点：`.`
+ 小括号：`（）`
+ 中括号：`[]`
+ 大括号：`{}`

**关键字**

我们的语言拥有以下这些关键字：

+ 类型：int  float  void  function
+ 流程控制：if  else  while  return  break  continue
+ 标准输入输出：cin  cout

**变量名**

我们对变量名做出如下规定：

+ 以字母或下划线开头
+ 由字母或数字或下划线组成
+ 不能与关键字重复
+ 用正则表达式`[_a-zA-Z][0-9_a-zA-Z]*`来识别变量名

**整数和浮点数**

+ 整数：用正则表达式`(0|([1-9]{digit}*))`来识别整数
+ 浮点数：用正则表达式`(0|([1-9]{digit}*))"."[0-9]*`来识别浮点数

**格式化输入输出**

+ 我们的输入输出格式是`cin("%d", &a);`
+ 为了达到输入输出的目的，我们需要匹配“%d”，用正则表达式`\"(\\.|[^"\\])*\"`来匹配格式化输入输出

### 3.2 语法分析

我们主要采用了yacc来帮助我们完成语法分析。即输入一串token流，匹配对应的语法，并转换为AST中的节点。具体的代码见parser.y。

以下是所有token流中的终结符：

```
%token<type_int> INT
%token<type_float> FLOAT
%token<type_str> ID FormatStr
%token<type_token> EQUAL LE GE NE LESS GREATER
%token<type_token> ASSIGN
%token<type_token> OR AND NOT
%token<type_token> ADD MINUS MULT DIV
%token<type_token> QUOT PERCENT LOGICAND LINEBREAK PERCENTD PERCENTF
%token<type_token> SEMI COMMA DOT LP RP LB RB LC RC
%token<type_token> IF ELSE WHILE TYPEINT TYPEFLOAT TYPEVOID RETURN BREAK CONTINUE FUNCTION INPUT OUTPUT
```

以下是所有非终结符，他们的类型都是AST树中的节点：

```
%type <type_ast>    Program Def_List Fun_List Var Var_List Fun Fun_Var_List Fun_Var VarDec
%type <type_ast>    AST_Type CBlock Stmt_List Stmt IF_Stmt If_Else_Stmt While_Stmt Input_Exp Output_Exp Exp Args
```

我们用到的语法规则如下：

**BASIC**

+ 整个程序分为变量定义部分和函数部分

  ```
  Program : Def_List Fun_List
  ```

+ 变量部分由很多变量定义例如`int a,b,c;`组成；函数部分由许多个函数组成

  ```
  //变量定义部分。
  Def_List -> Def_List Var SEMI| ɛ
  
  //函数部分
  Fun_List -> Fun_List Fun | ɛ
  ```

+ 变量定义语法

  ```
  //一条变量声明。例如int a,b,c[3]
  Var -> AST_Type Var_List
  
  //变量列表。例如a,b,c[3]
  Var_List -> Var_List COMMA VarDec | VarDec
  
  //单个变量。变量名可能是多维数组
  VarDec -> ID | VarDec LB INT RB        
  ```

+ 变量类型

  ```
  //主要有三种类型。int，float，void
  AST_Type -> TYPEINT | TYPEFLOAT | TYPEVOID
  ```

+ 函数部分语法

  ```
  //一个函数。函数可以有参或者无参，AST_Type是函数的返回值类型，CBlock是函数的主体部分
  Fun -> FUNCTION AST_Type ID LP Fun_Var_List RP CBlock |FUNCTION AST_Type ID LP RP CBlock 
  
  //函数参数列表。例如int a,float b[3]
  Fun_Var_List -> Fun_Var_List COMMA Fun_Var | Fun_Var
  
  //单个函数参数。例如int a
  Fun_Var -> AST_Type VarDec
  
  //函数主体部分。分为变量定义部分和语句部分（例如if，while等语句块）
  CBlock -> LC Def_List Stmt_List RC
  ```

**STATEMENT**

+ 语句列表

  ```
  //一个语句列表中可以有多个语句
  Stmt_List -> Stmt_List Stmt | Stmt
  ```

+ 语句

  ```
  //一个Statement可以是一个语句，也可以是一个语句段，即CBLOCK
  Stmt -> Exp SEMI
       | CBlock
       | RETURN SEMI
       | RETURN Exp SEMI
       | IF_Stmt
       | If_Else_Stmt
       | While_Stmt
       | Input_Exp SEMI 
       | Output_Exp SEMI 
  ```

+ 语句类型

  ```
  //if语句块
  IF_Stmt -> IF LP Exp RP CBlock
  
  //if，else语句块
  If_Else_Stmt -> IF LP Exp RP CBlock ELSE CBlock
  
  //while循环语句块
  While_Stmt -> WHILE LP Exp RP CBlock
  
  //输入语句块
  Input_Exp -> INPUT LP FormatStr COMMA Args RP
  
  //输出语句块
  Output_Exp -> OUTPUT LP FormatStr COMMA Args RP | OUTPUT LP FormatStr RP
  ```

**EXPRESSION**

+ 表达式类型

  ```
  Exp -> Exp ASSIGN Exp   //赋值表达式
      | Exp AND Exp       //和运算表达式
      | Exp OR Exp        //或运算表达式
      | Exp ADD Exp       //加运算表达式
      | Exp MINUS Exp     //减运算表达式
      | Exp MULT Exp      //乘运算表达式
      | Exp DIV Exp       //除运算表达式
      | Exp EQUAL Exp     //==
      | Exp LE Exp        //<=
      | Exp GE Exp        //>=
      | Exp NE Exp        //!=
      | Exp LESS Exp      //<
      | Exp GREATER Exp   //>
      | LOGICAND Exp      //&
      | LP Exp RP         //()
      | MINUS Exp         //-
      | NOT Exp           //！
      | ID LP Args RP     //数组
      | ID LP RP          //调用无参函数
      | Exp LB Exp RB     //调用有参函数
      | Exp DOT Exp       //结构体
      | ID                //变量名
      | INT               //整数
      | FLOAT             //浮点数
  ```

### 3.3 AST

每个程序在经过parser.y解析之后都会生成一颗抽象语法树（AST）。具体代码见AST.h与AST.c。

#### 3.3.1 结构设计

AST的基础结构定义如下：

```c++
class baseAST{
public:
    static int staticID;     //节点自增ID，用来统计节点数量从而给id赋值
    unsigned id;             //唯一id标识
    std::string name;        //名称，用于存储和传递
    AST_Type type;           //节点类型
    AST_DataType dataType;   //节点数据类型
    unsigned childCnt;       //节点孩子节点数量 
    std::vector<baseAST *> children;   //存储孩子节点

    //构造函数
    baseAST();              
    baseAST(AST_Type type, AST_DataType dataType);
    baseAST(AST_Type type, std::string name);

    void Insert(baseAST *);   //插入孩子节点
    void print(void);         //打印AST
    void buildTable(Func *scope);   //构造符号表和函数表
    void scanTree(Func *scope);     //扫描AST
};
```

其中节点类型主要有以下几种：

```c++
typedef enum AST_Type
{
    T_root,      // 根节点
    T_none,      // 无类型
    T_operator,  // 操作符
    T_var,       // 变量
    T_fvar,      // 函数名
    T_const,     // 常量
    T_func,      // 函数
    T_defi,      // 定义
    T_list,      // 列表
    T_tydf,      // 类型
    T_expr,      // 表达式
    T_formatstr, // 规范输入输出
    T_block,     // 函数体
} AST_Type;
```

数据类型主要有以下几类：

```c++
typedef enum AST_DataType
{
    DT_nonedt,    //无类型
    DT_void,      //空值
    DT_integer,   //整数
    DT_float,     //浮点数
    DT_string,    //字符串
    DT_function,  //函数
} AST_DataType;
```

我们还把baseAST作为基类，继承了这个基类从而构造了一些常用的节点：

+ 操作符节点   operateNode

  拥有操作符这个成员变量

  ```c++
  typedef enum AST_Operator
  {
      O_AND,      //&&
      O_OR,       //||
      O_ADD,      //+
      O_MINUS,    //-
      O_MULT,     //*
      O_DIV,      // /
      O_EQUAL,    //==
      O_LE,       //<=
      O_GE,       //>=
      O_NE,       //!=
      O_LESS,     //<
      O_GREATER,  //>
      O_LOGICAND, //&
      O_MINUSDIGIT,//-
      O_NOT,      //!
      O_noneop,   //无类型
  } AST_Operator;
  
  class operatorNode : public baseAST{
  public:
  
      AST_Operator op;  /* only for operator */
      operatorNode(AST_Operator op, AST_DataType dataType);
      operatorNode(AST_Operator op, std::string name);
      ~operatorNode();
  };
  ```

+ 常量节点 constNode

  拥有常量值这个成员变量

  ```c++
  typedef union AST_Value
  {
      int integer;
      char *str;
      float floatt;
  } AST_Value;
  
  class constNode : public baseAST{
  public:
      AST_Value dvalue; /* only for const */
      constNode(int value, AST_DataType dataType);
      constNode(char *value, AST_DataType dataType);
      constNode(float value,AST_DataType dataType);
      ~constNode();
  };
  ```

#### 3.3.2 语法树构造

我们在parser.y中构造语法树节点，并将节点插入到语法树中。

下面举几个例子展现我们的构造语法树思路：

+ 例如如下的语法。最基础的程序由定义列表和函数列表组成，因此我们首先创建一个类型为根类型的AST节点，并将他命名为Program，并且给这个节点插入两个字节点——定义列表和函数列表。

  ```
  Program : Def_List Fun_List {
      $$ = new baseAST(AST_Type::T_root, "Program");
      head = $$;
      head->Insert($1);
      head->Insert($2);
  }
  ```

+ 在如下的语法中。我们构造了一个定义列表的节点，并且为这个字节点插入了许多变量定义的子节点。因此这个定义列表的节点的节点类型是list。

  ```
  Def_List : Def_List Var SEMI{$$=$1;$$->Insert($2);}
           |{$$=new baseAST(AST_Type::T_list,"Def_List");}
           ;
  ```

+ 在如下的语法中，这是一个加法表达式。我们首先构造了一个Operater类型为“+”的操作符节点，并且把两个要相加的表达式插入这棵树，作为“+”节点的左右两个子节点。

  ```
  Exp : Exp ADD Exp{$$=new operatorNode(AST_Operator::O_ADD,"ADD");$$->Insert($1);$$->Insert($3);}
  ```

#### 3.3.3 可视化成果展示

我们在生成抽象语法树之后，利用print函数将树的结构打印到json文件中。并利用react将树的结构可视化的展示在网页上。

下图是快速排序形成的抽象语法树。

![AST可视化](resources/AST可视化.jpg)

### 3.4 符号表和函数表 fzy
由于存在变量作用域的概念，在编译器中存在有局部变量和全局变量，全局变量需要定义在函数外部，在`main`函数中被定义，在其它文件中被声明。
在遍历抽象语法树的时候，我们会对符号表和函数表进行构建，每当扫描到新定义的元素的时候，需要将其加入符号表，当扫描到函数的时候，需要将其加入全局的函数符号表中。
由于不存在闭包的概念，函数中只可以存在陈述语句和变量定义，不能存在函数定义。

全局符号表是一个由符号名到变量属性的映射关系，定义如`std::map<std::string, Var *>`。
其中`Var`中存储了变量的名称、类别以及是否在定义后被使用过，具体的类型定义如下：
```c++
class Var {
public:
    std::string name;
    AST_DataType type;
    bool used; // default false
    Var(std::string name, AST_DataType type) : name(std::move(name)), type(type), used(false) {}
    ~Var() {}
};
```

类似地，函数符号表同样是一个由函数名称映射到函数属性信息入口的对应关系，定义如`std::map<std::string, Func *>`
每个函数属性包含了名称、种类、以及函数内部的局部变量，在构建符号表的过程中，建表函数`buildTable`需要传入当前作用域`Func *scope`，在调用时可以递归调用传入更深层的解析树的结构中。


```c++
class Func {
public:
    std::string name;
    AST_DataType type;
    std::map<std::string, Var *> localVars; // local variables in the specific function
    Func(std::string name, AST_DataType rtype): name(std::move(name)), type(rtype) {}
    ~Func(){}
};
```



### 3.5 中间代码生成 jwz


## 4 进阶主题 fzy

### 4.1 错误检测 
在我们的编译器中，定义有一套统一的通知信息输出规范，不同重要性的信息被分为三种级别，分别为最严重的错误：`ERROR`、警告：`WARNING`和通知：`INFO`，对于不同分析阶段产生的报错，我们的编译器都可以有效地进行提示，报错会附带有提示的等级。

#### 4.1.1 词法分析
在词法分析的过程中，lex程序会处理解析得到的错误token，例如identifier如果以数字开头，那么它是不合法的，在词法分析的过程中就会报错并终止编译。

#### 4.1.2 语法分析

在语法分析的过程中，

#### 4.1.3 AST构建

在AST构建的过程中，程序会对整个抽象语法树进行两次扫描，第一遍遍历会经过所有`AST_Type::T_Defi`类型的节点，通过遍历所有程序块中的定义部分，遍历并建立符号表，第二遍是遍历不会经过之前遍历过的部分，而会着重于`statement`语句中对符号表中的变量和定义调用是否是合法的。


### 4.2 宏展开 

### 4.3 变量优化
我们借鉴Golang的设计思想，对于某个作用域中的变量，如果我们在定义之后从来没有使用过它（包括对其赋值、参与算数或逻辑运算等操作），那么在编写程序时此变量应当被优化掉，以减少内存空间的消耗。
我们通过对抽象语法树的两遍遍历，第一次得到了程序中所有能够被用到的变量，第二次对表达式进行分析，得到未被使用的变量，在编译结束的阶段会抛出`WARNING`级别的信息，提示用户某个作用域内存在未使用的变量。
因此在符号表的项目中也需要加上标记是否变量被调用过的变量，在扫描结束后通过遍历符号表中的元素提示变量的使用情况。


## 5 测试样例

### 5.1 快速排序 lsh

#### 5.1.1 简介 

**输入:**第一行是一个整数 *N* ，满足 0 ⩽ *N* ⩽ 104 。接下来是 *N* 个整数，每个整数独占一行。整数xi满足−104 <xi<104 但不保证i!=j时xi!=xj。

**输出:**要求将输入的 *N* 个整数按非降序排序。计算结束后，需要将 *N* 个整数依次输出，每个数字独占 一行。注意最后一个数字输出后必须换行。

要求必须使用快速排序算法解决该问题，并且必须使用递归形式的实现。

#### 5.1.2 代码实现

```c--
/*quick sort*/
int data[1000];
function void qsort(int l,int r){
    int ll;
    int rr;
    int pivot;
    int tmp;

    ll = l;
    rr = r;
    pivot = data[l];

    if(l>r){
        return;
    }
    while(ll<rr){
        while(data[ll]<pivot){
            ll=ll+1;
        }
        while(data[rr]>pivoit){
            rr=rr-1;
        }
        if(ll<=rr){
            if(data[ll]>=data[rr]){
                tmp=data[ll];
                data[ll]=data[rr];
                data[rr]=tmp;
            }
            ll=ll+1;
            rr=rr-1;
        }
    }
    qsort(l,rr);
    qsort(ll,r);
}
function int main(){
    int i;
    int n;
    i = 1;
    cin("%d",&n);
    while(i<=n){
        cin("%d",&data[i]);
        i=i+1;
    }
    qsort(1,n);
    i=1;
    while(i<=n){
        cout("%d\n",data[i]);
        i=i+1;
    }
    return 0;
}
```

#### 5.1.3 测试结果


### 5.2 矩阵乘法 jwz

#### 5.2.1 简介

#### 5.2.2 代码实现

```c--

```

#### 5.2.3 测试结果

### 5.3 选课助手 fzy

#### 5.3.1 简介

在选课助手的样例中，程序需要一行行读入学生的培养方案，之后对学生培养方案上的所有课程进行信息汇总，之后得到学生的均绩GPA，总共尝试的学分、已修的学分和剩余学分。

#### 5.3.2 代码实现

```c--

```

#### 5.3.3 测试结果 

## 6 心得体会

**刘舒菡：**这次编译原理大作业对我们而言是一次很大的挑战，我们进入到了一个全新的未知的领域，去探索了编译器底层的一些实现。非常感谢我的两个队友们，他们都非常的可靠！也正是靠我们共同的努力，才能将这个大作业比较完善地完成。在这个过程中，我收获了很多，对lex和yacc的用法更加熟练了，对编译器实现的底层原理更加了解了，也增强了我的团队合作能力。纵然我们的编译器还有进步的空间，但我在这次实验中收获了许多的成就感与满足感！

**范钊瑀：**为了顺利完成我们整体的编译器，我们需要将课堂中学到的理论知识和代码的实现联系起来，在不断的学习中，我对课堂中有关词法、语法分析的了解更加详细，对真实情况下的编译器的实现也更加了解。此外，在完成大作业的过程中，我也学习到了许多其它相关的知识，例如使用Git进行协作，使用CMake和Makefile管理工程，以及CI/CD等自动化工具。除了专业课的知识以外，工程管理和团队协作的知识也都是在合作中至关重要的环节。

**季文卓：**

