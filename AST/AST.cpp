#include "AST.h"
#include "PainterWindow.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;


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

    ofstream outfile;
    outfile.open("../visu/src/tree.json");
    outfile<<genJson(this).dump(2);
    outfile.close();
}


int baseAST::IDAccumulate = 0;

int main(int argc, char ** argv){
    baseAST * ast = new baseAST();
    ast->childCnt=2;
    ast->children.push_back(new varNode("a",DataType::DT_integer));
    ast->children.push_back(new constNode(1,DataType::DT_integer));

    ofstream outfile;
    outfile.open("../visu/src/1.json");
    outfile<<genJson(ast).dump(2);
    outfile.close();
    std::cout << genJson(ast).dump(2) << std::endl;


    return 0;
}
