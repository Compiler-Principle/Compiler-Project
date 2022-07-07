#include "AST.h"

// Constructor
baseAST::baseAST() {
    this->id=++staticID;
    this->type=AST_Type::T_none;
    this->dataType=AST_DataType::DT_nonedt;
    this->childCnt=0;
}

baseAST::baseAST(AST_Type type, AST_DataType dataType){
    this->id=++staticID;
    this->type=type;
    this->dataType=dataType;
    this->childCnt=0;
}


varNode::varNode(const char *name, AST_DataType dataType){
    this->id=++staticID;
    this->type=AST_Type::T_var;
    this->dataType=dataType;
    this->childCnt=0;
}


constNode::constNode(int value, AST_DataType dataType){
    this->id=++staticID;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.integer=value;
}

constNode::constNode(char *value, AST_DataType dataType){
    this->id=++staticID;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.str=value;
}
constNode::constNode(float value, AST_DataType dataType){
    this->id=++staticID;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.floatt=value;
}

constNode::~constNode(){
    delete this->dvalue.str;
}

operatorNode::operatorNode(AST_Operator op, std::string name){
    this->id=++staticID;
    this->type=AST_Type::T_operator;
    this->op=op;
    // this->dataType=dataType;
    this->name=name;
    this->childCnt=0;
}

void baseAST::Insert(baseAST *p){
    this->childCnt++;
    children.push_back(p);
}
const char *types[] = {"T_root",
                             "T_none",
                             "T_operator",  //2
                             "T_var", //3
                             "T_fvar",//4
                             "T_const", //5
                             "T_func",  //6
                             "T_defi",  //7
                             "T_list",  //8
                             "T_tydf",  //9
                             "T_expr",  //10
                             "T_formatstr",  //11
                             "T_block",  //12
                             "T_struct", //13
}; //
const char *datatypes[]={
    "DT_nonedt",   //0
    "DT_void",     //1
    "DT_integer",  //2
    "DT_float",    //3
    "DT_string",   //4
    "DT_function", //5
};
std::map<std::string, std::string> operatorMap{{"AND","&&"},{"OR","||"},{"ADD","+"},{"MINUS","-"},{"MULT","*"},{"DIV","÷"},{"EQUAL","=="},{"LE","<="},{"GE",">="},{"NE","!="},{"LESS","<"},{"GREATER",">"},{"LOGICAND","&"},{"MINUSDIGIT","-"},{"NOT","!"}};

json_t genJson(baseAST *ast){
    json_t treeRoot;
    treeRoot["id"]=ast->id;
    treeRoot["type"]=ast->type;
    treeRoot["dataType"]=ast->dataType;
    char n[100];
    switch (ast->type)
    {
    case 0: 
        sprintf(n, "%d %s", ast->id,  ast->name.c_str());
        break;
    case 1: 
        sprintf(n, "%d none:%s", ast->id,  ast->name.c_str());
        break;
    case 2: 
        sprintf(n, "%d oper:%s", ast->id,operatorMap[ast->name].c_str());
        break;
    case 3:
        sprintf(n, "%d var:%s", ast->id, ast->name.c_str());
        break;
    case 4: 
        sprintf(n, "%d fvar:%s", ast->id, ast->name.c_str());
        break;
    case 5: {
        constNode* constnode=(constNode*)ast;
        if(ast->dataType==2)
            sprintf(n, "%d const:%d %s", ast->id, constnode->dvalue.integer,datatypes[ast->dataType]);
        else if(ast->dataType==3)
            sprintf(n, "%d const:%f %s", ast->id, constnode->dvalue.floatt,datatypes[ast->dataType]);
        else if(ast->dataType==4)
            sprintf(n, "%d const:%s %s", ast->id, constnode->dvalue.str,datatypes[ast->dataType]);
    }
        break;
    case 6: 
        sprintf(n, "%d func:%s", ast->id, ast->name.c_str());
        break;
    case 7:
        sprintf(n, "%d defi", ast->id);
        break;
    case 8:
        sprintf(n, "%d %s", ast->id, ast->name.c_str());
        break;
    case 9:
        sprintf(n, "%d typdef:%s", ast->id, ast->name.c_str());
        break;
    case 10:
        sprintf(n, "%d exp:%s", ast->id, ast->name.c_str());
        break;
    case 11:
        sprintf(n, "%d formatstr:%s", ast->id, ast->name.c_str());
        break;
    case 12:
        sprintf(n, "%d CBLOCK", ast->id);
        break;
    case 13:
        sprintf(n, "%d STRUCT:%s", ast->id,ast->name.c_str());
        break;        
    default:
        sprintf(n, "id:{%d} name:%s type:%s dataType:%s", ast->id, ast->name.c_str(),types[ast->type],datatypes[ast->dataType]);
        break;
    }
    // if(ast->dataType==0)
    //     sprintf(n, "id:{%d} name:%s type:%s", ast->id,  ast->name.c_str(),types[ast->type]);
    // else
    //     sprintf(n, "id:{%d} name:%s type:%s dataType:%s", ast->id, ast->name.c_str(),types[ast->type],datatypes[ast->dataType]);
    std::string name(n);
    treeRoot["name"] = name;
    treeRoot["childCnt"]=ast->childCnt;
    json_t c = json_t::array();
    for(int i=0;i<ast->children.size();i++){
        c.push_back(genJson(ast->children[i]));
    }
    treeRoot["children"]=c;
    return treeRoot;
}



void baseAST::print() {
    std::ofstream outfile;
    outfile.open("./AST/visu/src/tree.json");
    outfile<<genJson(this).dump(2);
    outfile.close();
}

baseAST* fakeTree(){
    auto ast = new baseAST();
    ast->childCnt=2;
    ast->children.push_back(new varNode("a", AST_DataType::DT_integer));
    ast->children.push_back(new constNode(1, AST_DataType::DT_integer));
    return ast;
}

int baseAST::staticID = 0;

void baseAST::buildTable(Func *scope) {
    // Traverse among the tree to build the symbol table
    switch (this->type) {
        case AST_Type::T_var:
            // Insert the variable into the functino symbol table
            if(scope == nullptr){
                // Global var
                if(globalVars.find(this->name) != globalVars.end()){
                    // Duplicate variable
                    info(InfoLevel::ERROR, "Duplicate global variable: " + this->name);
                }else{
//                    info(InfoLevel::INFO, "Create global var " + this->name);
                    globalVars[this->name] = new Var(this->name,this->dataType);
                }
            }
            else {
                // Local var
                if(scope->localVars.find(this->name) != scope->localVars.end()) {
                    // Duplicate variable
                    info(InfoLevel::ERROR, "Duplicate local variable: " + this->name + " in " + scope->name);
                }else {
//                    info(InfoLevel::INFO, "Create func var " + this->name);
                    scope->localVars[this->name] = new Var(this->name,this->dataType);
                }
            }
            break;
        case AST_Type::T_func:
            // Insert the constant into the globalFuncs symbol table
            if(scope != nullptr){
                info(InfoLevel::ERROR, "CLOSURE IS INVALID!");
            }
            else{
                // global functions
                if(globalFuncs.find(this->name) != globalFuncs.end()){
                    // Duplicate function
                    info(InfoLevel::ERROR, "Duplicate global function: " + this->name);
                }else{
                    auto tmpFunc = new Func(this->name, this->dataType);
                    globalFuncs[this->name] = tmpFunc;
                    tmpFunc->type = this->children.at(0)->dataType; // return type of func
                    // Build the function symbol table
                    // Fun : FUNCTION AST_Type ID LP Fun_Var_List RP CBlock{$$=new baseAST(AST_Type::T_func,$3);delete $3;$$->Insert($2);$$->Insert($5);$$->Insert($7);}
                    // |FUNCTION AST_Type ID LP RP CBlock{$$=new baseAST(AST_Type::T_func,$3);delete $3;$$->Insert($2);$$->Insert($6);}
                    if(this->children.size() == 2) {
                        // No func parameter
                        // AST_Type CBlock
                        this->children.at(0)->buildTable(tmpFunc);
                        this->children.at(1)->buildTable(tmpFunc);
                    }
                    else if(this->children.size() == 3) {
                        // Has func parameter
                        // AST_Type Fun_Var_List CBlock
                        this->children.at(0)->buildTable(tmpFunc);
                        this->children.at(1)->buildTable(tmpFunc);
                        this->children.at(2)->buildTable(tmpFunc);
                    }
                }

            }

            break;

        case AST_Type::T_defi:
            // Definition of a specific var
            // Var : AST_Type Var_List
            this->children.at(1)->dataType = this->children.at(0)->dataType;
            this->dataType = this->children.at(0)->dataType;

            this->children.at(1)->buildTable(scope);

            break;
        case AST_Type::T_root:
            // Root only has 2 children
            this->children.at(0)->buildTable(nullptr);
            this->children.at(1)->buildTable(nullptr);
            break;
        case AST_Type::T_block:
            // Block : AST_Type AST_Type
            this->children.at(0)->buildTable(scope);
//            this->children.at(1)->buildTable(scope);
            break;

        case AST_Type::T_list:
            // List has many types

            if(this->name == "Def_List"){
                // Definition list
                // Def_list : Def_list Var SEMI
                // 每个 Var 是类似于 int a,b,c的形式
                for(auto &t: this->children){
                    t->buildTable(scope);
                }
            }
            else if(this->name == "Var_List"){
                // Var list
                // Var_List : Var_List COMMA VarDec
                for(auto &t: this->children){
                    t->dataType = this->dataType;
                    t->buildTable(scope);
                }
            }
            else if(this->name == "Fun_List"){
                // Fun_list
                for(auto &t: this->children){
                    t->buildTable(nullptr);
                }
            }
            else if(this->name == "Fun_Var_List"){
                // Fun_Var_List
                // Fun_Var_List: Fun_Var_List COMMA
                for(auto &t: this->children){
                    t->buildTable(scope);
                }

            }
            else if(this->name == "Stmt_List"){
                for(auto &t: this->children){
                    t->buildTable(scope);
                }
            }

            else if(this->name == "Args"){
                for(auto &t: this->children){
                    t->buildTable(scope);
                }
            }

            break;

        case AST_Type::T_fvar:
            // function var
            // Fun_Var:AST_Type VarDec
            if(scope == nullptr){
                info(InfoLevel::ERROR, "FUNCTION IS NULLPTR!");
                return;
            }
            else {
                // Local var
                if(scope->localVars.find(this->name) != scope->localVars.end()) {
                    // Duplicate variable
                    info(InfoLevel::ERROR, "Duplicate local variable with function var " + this->name);
                    return;
                }else {
                    // add the function vars to local vars
                    std::string n = this->children.at(1)->name;
//                    info(InfoLevel::INFO, "Create func param var " + n + " in function " + scope->name);
                    scope->localVars[n] = new Var(this->name,this->dataType);
                }
            }

            this->children.at(1)->dataType = this->children.at(0)->dataType;
            this->dataType = this->children.at(0)->dataType;
            break;
        case AST_Type::T_tydf:
            break;
        case AST_Type::T_expr:
            // Expression
            for(auto &t: this->children){
                t->buildTable(scope);
            }
            break;
        case AST_Type::T_operator:
            break;
        default:
            break;
        
    }

}

void baseAST::scanTree(Func *scope) {
    // Traverse among the tree to build the symbol table
    switch (this->type) {
        case AST_Type::T_var:
            if(scope){
                if(scope->localVars.find(this->name) != scope->localVars.end()){
                    // Duplicate variable
//                    info(InfoLevel::INFO, "Used local var " + this->name);
                    scope->localVars[this->name]->used = true;
                }
            }
//            else {
                if(globalVars.find(this->name) != globalVars.end()){
                    // Duplicate variable
//                info(InfoLevel::INFO, "Used global var " + this->name);
                    globalVars[this->name]->used = true;
                }
                else{
                    info(InfoLevel::ERROR, "Undefined variable " + this->name);
                }
//            }
            break;
        case AST_Type::T_func:
            // Insert the constant into the globalFuncs symbol table
            for(auto &t: this->children){
                t->scanTree(globalFuncs[this->name]);
            }
            break;

        case AST_Type::T_defi:
            // Definition of a specific var
            // Var : AST_Type Var_List
            this->children.at(1)->dataType = this->children.at(0)->dataType;
            this->dataType = this->children.at(0)->dataType;

            this->children.at(1)->scanTree(scope);

            break;
        case AST_Type::T_root:
            // Root only has 2 children
            this->children.at(1)->scanTree(nullptr);
            break;
        case AST_Type::T_block:
            // Block : AST_Type AST_Type
//            this->children.at(0)->scanTree(scope);
            this->children.at(1)->scanTree(scope);
            break;

        case AST_Type::T_list:
            // List has many types

            if(this->name == "Def_List"){
                // Definition list
                // Def_list : Def_list Var SEMI
                // 每个 Var 是类似于 int a,b,c的形式
                for(auto &t: this->children){
                    t->scanTree(scope);
                }
            }
            else if(this->name == "Var_List"){
                // Var list
                // Var_List : Var_List COMMA VarDec
                for(auto &t: this->children){
                    t->dataType = this->dataType;
                    t->scanTree(scope);
                }
            }
            else if(this->name == "Fun_List"){
                // Fun_list
                for(auto &t: this->children){
                    t->scanTree(nullptr);
                }
            }
            else if(this->name == "Fun_Var_List"){
                // Fun_Var_List
                // Fun_Var_List: Fun_Var_List COMMA
                for(auto &t: this->children){
                    t->scanTree(scope);
                }

            }
            else if(this->name == "Stmt_List"){
                for(auto &t: this->children){
                    t->scanTree(scope);
                }
            }

            else if(this->name == "Args"){
                for(auto &t: this->children){
                    t->scanTree(scope);
                }
            }

            break;

        case AST_Type::T_fvar:
            // function var
            // Fun_Var:AST_Type VarDec

            break;
        case AST_Type::T_tydf:
            break;
        case AST_Type::T_expr:
            // Expression
            for(auto &t: this->children){
                t->scanTree(scope);
            }
            break;
        case AST_Type::T_operator:
            // local var
            for(baseAST * &t: this->children){
                if(scope){
                    if(scope->localVars.find(t->name) != scope->localVars.end()){
                        // Duplicate variable
                        info(InfoLevel::INFO, "Used local var " + t->name);
                        scope->localVars[t->name]->used = true;
                    }
                }
                if(globalVars.find(t->name) != globalVars.end()){
                    // Duplicate variable
                    info(InfoLevel::INFO, "Used global var " + t->name);
                    globalVars[t->name]->used = true;
                }
                else{
                    if(t->name != "cinResult"){
                        info(InfoLevel::ERROR, "Undefined variable " + t->name);
                    }
                }
            }
            break;
        default:
            break;

    }

}

baseAST::baseAST(AST_Type type, std::string name) : type(type), name(std::move(name)) {
    this->id=++staticID;
    this->childCnt=0;
    this->dataType=AST_DataType::DT_nonedt;
}

void printTable(){
    std::cout <<  globalVars.size() << " Global Variables: ";
    for(auto it=globalVars.begin();it!=globalVars.end();it++){
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    std::cout << globalFuncs.size() << " Global Functions: " << std::endl;
    for(auto it=globalFuncs.begin();it!=globalFuncs.end();it++){
        std::cout << it->first << " ";
        std::cout << it->second->localVars.size() << " local Vars: ";
        for(auto &var :it->second->localVars){
            std::cout << var.first << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void checkVars(){
//    std::cout << "Checking Global Vars: " << std::endl;
    for(auto &i: globalVars){
        if(!i.second->used){
            info(InfoLevel::WARNING, "Unused global symbol: " + i.first);
        }
    }
    for(auto &f: globalFuncs){
//        std::cout << "Checking function: " << f.first << std::endl;
        for(auto &i: f.second->localVars){
            if(!i.second->used){
                info(InfoLevel::WARNING, "Unused local symbol " + i.first + " in function " + f.first);
            }
        }
    }

}

// int main(int argc, char ** argv){

//    std::map<std::string, Var *> varTable;
//    std::map<std::string, Func *> funcTable;


//    return 0;
// }
