#include "AST.h"
#include "PainterWindow.h"
#include <iostream>
#include <vector>
#include <cstring>
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

void baseAST::print() {
    int a = 0, &i = a;
    QApplication app(i, nullptr);

    PainterWindow w(this);
    w.show();
    app.exec();

}
