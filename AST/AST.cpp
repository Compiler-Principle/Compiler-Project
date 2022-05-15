#include "AST.h"
#include <map>

using namespace std;

// Constructor
baseAST::baseAST() {
    this->id=++IDAccumulate;
    this->type=AST_Type::T_none;
    this->dataType=AST_DataType::DT_nonedt;
    this->childCnt=0;
}

baseAST::baseAST(AST_Type type, AST_DataType dataType){
    this->id=++IDAccumulate;
    this->type=type;
    this->dataType=dataType;
    this->childCnt=0;
}



varNode::varNode(const char *name, AST_DataType dataType){
    this->id=++IDAccumulate;
    this->type=AST_Type::T_var;
    this->dataType=dataType;
    this->childCnt=0;
}


constNode::constNode(int value, AST_DataType dataType){
    this->id=++IDAccumulate;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.integer=value;
}

constNode::constNode(char *value, AST_DataType dataType){
    this->id=++IDAccumulate;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.str=value;
}
constNode::constNode(float value, AST_DataType dataType){
    this->id=++IDAccumulate;
    this->type=AST_Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.floatt=value;
}

constNode::~constNode(){
    delete this->dvalue.str;
}

operatorNode::operatorNode(AST_Operator op, std::string name){
    this->id=++IDAccumulate;
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
                             "T_block", //12
}; //
const char *datatypes[]={
    "DT_nonedt",   //0
    "DT_void",     //1
    "DT_integer",  //2
    "DT_float",    //3
    "DT_string",   //4
    "DT_function", //5
};
map<string,string> operatorMap{{"AND","&&"},{"OR","||"},{"ADD","+"},{"MINUS","-"},{"MULT","*"},{"DIV","÷"},{"EQUAL","=="},{"LE","<="},{"GE",">="},{"NE","!="},{"LESS","<"},{"GREATER",">"},{"LOGICAND","&"},{"MINUSDIGIT","-"},{"NOT","!"}};

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
    std::cout << "printing tree" << std::endl;
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

int baseAST::IDAccumulate = 0;
std::map<std::string, Var *> globalVars;
std::map<std::string, Func *> globalFuncs;

void baseAST::buildTable(Func *scope) {
    // Traverse among the tree to build the symbol table
    switch (this->type) {
        case AST_Type::T_var:
            // Insert the variable into the functino symbol table
            if(scope == nullptr){
                // Global var
                if(globalVars.find(this->name) == globalVars.end()){
                    // Duplicate variable
                    info(InfoLevel::ERROR, "Duplicate global variable: " + this->name);
                }else{
                     globalVars[this->name] = new Var(this->name,this->dataType);
                }
            }
            else {
                // Local var
                if(scope->localVars.find(this->name) != scope->localVars.end()) {
                    // Duplicate variable
                    info(InfoLevel::ERROR, "Duplicate local variable: " + this->name);
                }else {
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
                    globalFuncs[this->name] = new Func(this->name,this->dataType);
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
            this->children.at(1)->buildTable(scope);
            break;

        case AST_Type::T_list:
            // List has many types
            if(this->name == "Def_list"){
                // Definition list
                // Def_list : Def_list Var SEMI
                // 每个 Var 是类似于 int a,b,c的形式
                for(auto &t: this->children){
                    t->buildTable(scope);
                }
            }
            else if(this->name == "Var_list"){
                // Var list
                // Var_List : Var_List COMMA VarDec
                for(auto &t: this->children){
                    t->dataType = this->dataType;
                    t->buildTable(scope);
                }
            }
            else if(this->name == "Fun_list"){
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
            else if(this->name == "Stmt_list"){
                for(auto &t: this->children){
                    t->buildTable(nullptr);
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
                    scope->localVars[this->name] = new Var(this->name,this->dataType);
                }
            }

            this->children.at(1)->dataType = this->children.at(0)->dataType;
            this->dataType = this->children.at(0)->dataType;
            break;

        case AST_Type::T_expr:
            break;
        case AST_Type::T_operator:
            break;
        default:

            break;
        
    }

}

baseAST::baseAST(AST_Type type, std::string name) : type(type), name(std::move(name)) {
    this->id=++IDAccumulate;
    this->childCnt=0;
    this->dataType=AST_DataType::DT_nonedt;
}



// int main(int argc, char ** argv){

//    std::map<std::string, Var *> varTable;
//    std::map<std::string, Func *> funcTable;


//    return 0;
// }
