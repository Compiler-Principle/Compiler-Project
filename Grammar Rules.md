# Grammar Rules
## Tokens

%token EQUAL LE GE NE LESS GREATER

%token ASSIGN

%token OR AND NOT

%token ADD MINUS MULT DIV

%token QUOT PERCENT LOGICAND LINEBREAK PERCENTD PERCENTF

%token SEMI COMMA DOT LP RP LB RB LC RC

%token IF ELSE WHILE TYPEINT TYPEFLOAT TYPEVOID RETURN BREAK CONTINUE FUNCTION INPUT OUTPUT

%token FLOAT INT ID

## BNF
### Basic
// 看test里是先定义变量再定义函数

    Program -> Def_list Fun_List 

// 定义一系列变量，Var：诸如**int a**，或是**float b[10]**等，由type和ID组成

    Def_List -> Def_List Var | ɛ

    Fun_List -> Fun_List Fun | ɛ

//函数由函数定义和函数体组成

    Fun -> FunDec CBlock

// CBLOCK 是由大括号包围成的表达式代码段，内部有变量定义和表达式语句，因此暂不支持闭包

// CBLOCK就是NJU那份C定义里的**CompSt**
    CBLOCK -> LC Def_List Stmt_List RC

    Type -> TYPEINT | TYPEFLOAT | TYPEVOID

### Declarations

// 变量定义，支持数组，这里数组的方括号中需要填一个int

    VarDec -> ID | VarDec LB INT RB

//FunDec为函数定义，包括有参数的和无参数的

//TODO 这里可能有reduce-reduce conflict

    FunDec -> FUNCTION Type ID LP Var_List RP | FUNCTION Type ID LP RP

// 变量序列，由多个变量组成，诸如**int a,float b[10]**

    Var_List -> Var COMMA Var_List | Var

    Var -> Type VarDec

### Statements

    If_Stmt -> IF LP Exp RP CBLOCK

    If_Else_Stmt -> IF LP Exp RP CBLOCK ELSE CBLOCK

    While_Stmt -> WHILE LP Exp RP CBLOCK

    Stmt_List -> Stmt StmtList | ɛ

// 一个Statement可以是一个语句，也可以是一个语句段，即CBLOCK

    Stmt -> Exp SEMI 
    | CBLOCK 
    | RETURN Exp SEMI 
    | If_Stmt
    | If_Else_Stmt
    | While_Stmt
    | Input_Exp
    | Output_Exp

### Expression
//Format_Str作为格式化输入，感觉需要修改lex得到双引号的位置

    Input_Exp -> INPUT LP Format_Str COMMA Var_List RP

    Output_Exp -> OUTPUT LP Format_Str COMMA Var_List RP

    Exp -> Exp ASSIGN Exp 
    | Exp AND Exp 
    | Exp OR Exp 
    | Exp NOT Exp 
    | Exp ADD Exp 
    | Exp MINUS Exp 
    | Exp MULT Exp 
    | Exp DIV Exp 
    | Exp EQUAL Exp
    | Exp LE Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp LESS Exp
    | Exp GREATER Exp
    | LP Exp RP 
    | MINUS Exp 
    | NOT Exp 
    | ID LP Args RP 
    | ID LP RP 
    | Exp LB Exp RB 
    | Exp DOT ID 
    | ID 
    | INT
    | FLOAT
    
    Args -> Exp COMMA Args | Exp