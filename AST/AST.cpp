#include "AST.h"


//using namespace std;

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

json_t genJson(baseAST *ast){
    json_t treeRoot;
    treeRoot["id"]=ast->id;
    treeRoot["type"]=ast->type;
    treeRoot["dataType"]=ast->dataType;
    char n[100];
    sprintf(n, "id:{%d} type:{%d} dataType:{%d}", ast->id, ast->type, ast->dataType);
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
        case AST_Type::T_operator:
            break;
        case AST_Type::T_defi:
            // Definition of a specific var
            // Var : AST_Type VarDec
            this->children.at(1)->dataType = this->children.at(0)->dataType;
            this->dataType = this->children.at(0)->dataType;

            this->children.at(1)->buildTable(scope);
        
            break;
        case AST_Type::T_root:
            // Root only has 2 children
            this->children.at(0)->buildTable(nullptr);
            this->children.at(1)->buildTable(nullptr);
        default:

            break;
        
    }

}

baseAST::baseAST(AST_Type type, std::string name) : type(type), name(std::move(name)) {
    this->id=++IDAccumulate;
    this->childCnt=0;
}



// int main(int argc, char ** argv){

//    std::map<std::string, Var *> varTable;
//    std::map<std::string, Func *> funcTable;


//    return 0;
// }
