#ifndef GEN_H
#define GEN_H

#include "../AST/AST.h" 
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <iostream>
#include <string>

using namespace llvm;
using namespace std;

void InitIOFunc();
void genCode(baseAST *ast);
void genBC();

void genStmt(baseAST *ast, IRBuilder<> funBuilder);
Type *getType(string type);
Value *getDefaultValue(string type);
Function *genPrototype(baseAST *ast);
Function *genFunc(baseAST *ast);
void genStmt(baseAST *ast, IRBuilder<> funBuilder);
Value *genExp(baseAST *ast, IRBuilder<> funBuilder);

#endif