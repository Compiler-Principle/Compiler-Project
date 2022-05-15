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

void genCode(baseAST *ast);
Function *genFun(baseAST *ast);
Value *genValue(baseAST *ast);
Type *getType(string type);
void genBC();

#endif