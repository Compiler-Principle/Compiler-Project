#include "AST.h"
#include "PainterWindow.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

//using namespace std;

// Constructor
baseAST::baseAST() {
    this->id=++IDAccumulate;
    this->type=Type::T_none;
    this->dataType=DataType::DT_nonedt;
    this->childCnt=0;
}

baseAST::baseAST(Type type,DataType dataType){
    this->id=++IDAccumulate;
    this->type=type;
    this->dataType=dataType;
    this->childCnt=0;
}



varNode::varNode(const char *name,DataType dataType){
    this->id=++IDAccumulate;
    this->type=Type::T_var;
    this->dataType=dataType;
    this->childCnt=0;
}


constNode::constNode(int value,DataType dataType){
    this->id=++IDAccumulate;
    this->type=Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.integer=value;
}

constNode::constNode(char *value,DataType dataType){
    this->id=++IDAccumulate;
    this->type=Type::T_const;
    this->dataType=dataType;
    this->childCnt=0;
    this->dvalue.str=value;
}

constNode::~constNode(){
    delete this->dvalue.str;
}

operatorNode::operatorNode(Operator op,DataType dataType){
    this->id=++IDAccumulate;
    this->type=Type::T_operator;
    this->dataType=dataType;
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
    treeRoot["name"] = "11";
    treeRoot["childCnt"]=ast->childCnt;
    json_t c = json_t::array();
    for(int i=0;i<ast->childCnt;i++){
        c.push_back(genJson(ast->children[i]));
    }
    treeRoot["children"]=c;
    return treeRoot;
}



void baseAST::print() {
    int a = 0, &i = a;

    std::ofstream outfile;
    outfile.open("../visu/src/tree.json");
    outfile<<genJson(this).dump(2);
    outfile.close();
}

baseAST* fakeTree(){
    auto ast = new baseAST();
    ast->childCnt=2;
    ast->children.push_back(new varNode("a",DataType::DT_integer));
    ast->children.push_back(new constNode(1,DataType::DT_integer));
    return ast;
}

int baseAST::IDAccumulate = 0;
std::map<std::string, Var *> globalVars;
std::map<std::string, Func *> globalFuncs;

void baseAST::buildTable(Func *scope) {
    // Traverse among the tree to build the symbol table
    switch (this->type) {
        case Type::T_var:
            // Insert the variable into the functino symbol table
            if(scope == nullptr){
                // Global var
                if(globalVars.find(this->name) == globalVars.end()){
                    // Duplicate variable
                    std::cerr << "[Error]: Duplicate global variable " << this->name << std::endl;
                }else{
                     globalVars[this->name] = new Var(this->name,this->dataType);
                }
            }
            else {
                // Local var
                if(scope->localVars.find(this->name) != scope->localVars.end()) {
                    // Duplicate variable
                    std::cerr << "Error: Duplicate local variable " << this->name << std::endl;
                }else {
                    scope->localVars[this->name] = new Var(this->name,this->dataType);
                }
            }
            break;
        case Type::T_func:
            // Insert the constant into the globalFuncs symbol table
            if(scope != nullptr){
                std::cerr << "[Error]: CLOSURE IS INVALID!" << std::endl;
            }

            break;
        default:

            break;
        
    }

}

int main(int argc, char ** argv){

    std::map<std::string, Var *> varTable;
    std::map<std::string, Func *> funcTable;

    std::cerr << "[Error]: CLOSURE IS INVALID!" << std::endl;




    return 0;
}
