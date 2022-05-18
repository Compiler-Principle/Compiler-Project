#include "gen.h"

LLVMContext TheContext;
Module TheModule("main", TheContext);
IRBuilder<> Builder(TheContext);

map<string, Function *> functions;
map<string, Value *> globalVariables;
map<string, vector<int>> globalArray;

Function *funcPrintf;
Function *funcScanf;

template <class T>
string printValue(T *t) {
    string str;
    raw_string_ostream stream(str);
    t->print(stream);
    return str;
}

void InitIOFunc() {
    vector<Type *> putsArgs;
    putsArgs.push_back(Builder.getInt8Ty()->getPointerTo());
    ArrayRef<Type *> argsRef(putsArgs);

    FunctionType *putsType = FunctionType::get(Builder.getInt32Ty(), argsRef, true);
    funcPrintf = Function::Create(putsType, Function::ExternalLinkage, Twine("printf"), TheModule);
    funcScanf = Function::Create(putsType, Function::ExternalLinkage, Twine("__isoc99_scanf"), TheModule);
}

Type *getType(string type) {
    if(type == "int") {
        return Type::getInt32Ty(TheContext);
    }
    else if(type == "float") {
        return Type::getDoubleTy(TheContext);
    }
    else if(type == "void") {
        return Type::getVoidTy(TheContext);
    }
    return Type::getVoidTy(TheContext);
}

Value *getDefaultValue(string type) {
    if(type == "int") {
        // return ConstantInt::get(Type::getInt32Ty(TheContext), 0);
        // return Builder.CreateAlloca(Type::getInt32Ty(TheContext));
    }
    else if(type == "float") {
        // return ConstantFP::get(Type::getFloatTy(TheContext), 0);
        // return Builder.CreateAlloca(Type::getFloatTy(TheContext));
    }
    return ConstantInt::get(Type::getInt32Ty(TheContext), 0);
    // return Builder.CreateAlloca(Type::getInt32Ty(TheContext));
}

Function *genPrototype(baseAST *ast) {
    baseAST *retAST, *argAST, *blockAST;
    
    retAST = ast->children[0];
    if(ast->childCnt == 3) {
        argAST = ast->children[1];
        blockAST = ast->children[2];
    }
    else {
        argAST = nullptr;
        blockAST = ast->children[1];
    }

    // 函数返回类型
    Type *returnType = getType(retAST->name);

    // 函数参数
    vector<Type *> funArgs;
    if(argAST != nullptr) {
        for(auto arg : argAST->children) {
            funArgs.push_back(getType(arg->children[0]->name));
        }
    }

    FunctionType *funcType = FunctionType::get(returnType, funArgs, false);
    Function *fun = Function::Create(funcType, Function::ExternalLinkage, Twine(ast->name), TheModule);

    functions[ast->name] = fun;
    return fun;
}

Function *genFunc(baseAST *ast) {
    baseAST *retAST, *argAST, *blockAST;
    
    retAST = ast->children[0];
    if(ast->childCnt == 3) {
        argAST = ast->children[1];
        blockAST = ast->children[2];
    }
    else {
        argAST = nullptr;
        blockAST = ast->children[1];
    }
    
    Function *fun = functions[ast->name];
    BasicBlock *funBlock = BasicBlock::Create(TheContext, "entry", fun);
    IRBuilder<> funBuilder(funBlock);

    for(auto stmt : blockAST->children[1]->children) genStmt(stmt, funBuilder);

    return fun;
}

void genStmt(baseAST *ast, IRBuilder<> funBuilder) {
    if(ast->name == "ASSIGN") {
        genExp(ast, funBuilder);
    }
    else if(ast->name == "Input_Exp") {
        vector<Value *> args;
        args.push_back(funBuilder.CreateGlobalStringPtr(ast->children[0]->name.substr(1, ast->children[0]->name.length() - 2)));
        for(int i = 0;i < ast->children[1]->childCnt;i++) {
            auto arg = genExp(ast->children[1]->children[i], funBuilder);
            args.push_back(arg);
        }
        ArrayRef<Value *> argsRef(args);

        funBuilder.CreateCall(funcScanf, argsRef);
    }
    else if(ast->name == "Output_Exp") {
        vector<Value *> args;
        args.push_back(funBuilder.CreateGlobalStringPtr(ast->children[0]->name.substr(1, ast->children[0]->name.length() - 2)));
        for(int i = 0;i < ast->children[1]->childCnt;i++) {
            auto arg = genExp(ast->children[1]->children[i], funBuilder);
            args.push_back(arg);
        }
        ArrayRef<Value *> argsRef(args);

        funBuilder.CreateCall(funcPrintf, argsRef);
    }
    else if(ast->name == "return") {
        if(ast->childCnt == 0) funBuilder.CreateRet(ConstantInt::get(Type::getInt32Ty(TheContext), 0));
        else funBuilder.CreateRet(genExp(ast->children[0], funBuilder));
    }
}

Value *genExp(baseAST *ast, IRBuilder<> funBuilder) {
    if(ast->name == "constint") {
        return ConstantInt::get(Type::getInt32Ty(TheContext), ((constNode *) ast)->dvalue.integer);
    }
    else if(ast->name == "constfloat") {
        return ConstantFP::get(Type::getDoubleTy(TheContext), ((constNode *) ast)->dvalue.floatt);
    }
    else if(ast->type == T_var) {
        return funBuilder.CreateLoad(globalVariables[ast->name]);
    }
    else if(ast->name == "ASSIGN") {
        return funBuilder.CreateStore(genExp(ast->children[1], funBuilder), globalVariables[ast->children[0]->name]);
    }
    else if(ast->name == "AND") {
        return funBuilder.CreateAnd(genExp(ast->children[0], funBuilder), genExp(ast->children[1], funBuilder));
    }
    else if(ast->name == "OR") {
        return funBuilder.CreateOr(genExp(ast->children[0], funBuilder), genExp(ast->children[1], funBuilder));
    }
    else if(ast->name == "ADD") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFAdd(l, r);
        else return funBuilder.CreateAdd(l, r);
    }
    else if(ast->name == "MINUS") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFSub(l, r);
        else return funBuilder.CreateSub(l, r);
    }
    else if(ast->name == "MULT") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFMul(l, r);
        else return funBuilder.CreateMul(l, r);
    }
    else if(ast->name == "DIV") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFDiv(l, r);
        else return funBuilder.CreateSDiv(l, r);
    }
    else if(ast->name == "EQUAL") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpOEQ(l, r);
        else return funBuilder.CreateICmpEQ(l, r);
    }
    else if(ast->name == "LE") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpOLE(l, r);
        else return funBuilder.CreateICmpSLE(l, r);
    }
    else if(ast->name == "GE") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpOGE(l, r);
        else return funBuilder.CreateICmpSGE(l, r);
    }
    else if(ast->name == "NE") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpONE(l, r);
        else return funBuilder.CreateICmpNE(l, r);
    }
    else if(ast->name == "LESS") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpOLT(l, r);
        else return funBuilder.CreateICmpSLT(l, r);
    }
    else if(ast->name == "GREATER") {
        auto * l = genExp(ast->children[0], funBuilder);
        auto * r = genExp(ast->children[1], funBuilder);
        if(l->getType()->isDoubleTy() || r->getType()->isDoubleTy()) return funBuilder.CreateFCmpOGT(l, r);
        else return funBuilder.CreateICmpSGT(l, r);
    }
    else if(ast->name == "MINUSDIGIT") {
        auto * child = genExp(ast->children[0], funBuilder);
        if(child->getType()->isDoubleTy()) return funBuilder.CreateFSub(ConstantFP::get(Type::getDoubleTy(TheContext), 0), child);
        else return funBuilder.CreateSub(ConstantInt::get(Type::getInt32Ty(TheContext), 0), child);
    }
    else if(ast->name == "LOGICAND") {
        return globalVariables[ast->children[0]->name];
    }
    return ConstantInt::get(Type::getInt32Ty(TheContext), 1909);
}

void genGlobalVar(baseAST *ast) {
    string type = ast->children[0]->name;
    vector<baseAST *> vars = ast->children[1]->children;
    for(auto var : vars) {
        if(var->childCnt == 0) {
            TheModule.getOrInsertGlobal(var->name, getType(type));
            auto gVar = TheModule.getGlobalVariable(var->name);
            gVar->setLinkage(GlobalValue::CommonLinkage);
            if(type == "int") gVar->setInitializer(ConstantInt::get(Type::getInt32Ty(TheContext), 0));
            else if(type == "float") gVar->setInitializer(ConstantFP::get(Type::getDoubleTy(TheContext), 0));
            globalVariables[var->name] = gVar;
        }
        else {
            vector<int> arrayLen;
            int len = 1;
            for(auto child : var->children) {
                arrayLen.push_back(((constNode *) child)->dvalue.integer);
                len *= ((constNode *) child)->dvalue.integer;
            }

            Type *arrayType;
            Value *arrayLen;
            if(type == "int") arrayType = ArrayType::get(Type::getInt32Ty(TheContext), len);
            else if(type == "float") arrayType = ArrayType::get(Type::getDoubleTy(TheContext), len);
            auto *constInit = llvm::ConstantAggregateZero::get(arrayType);
            Value *array = new GlobalVariable(TheModule, arrayType, false, GlobalValue::CommonLinkage, constInit, var->name);

            globalVariables[var->name] = array;
            globalArray[var->name] = arrayLen;
        }
    }
}

void genCode(baseAST *ast) {
    baseAST *varAST = ast->children[0];
    for(auto child : varAST->children) genGlobalVar(child);

    baseAST *funAST = ast->children[1];
    for(auto child : funAST->children) genPrototype(child);
    for(auto child : funAST->children) genFunc(child);
}

void genBC() {
    std::error_code ErrInfo;
    raw_ostream *out = new raw_fd_ostream("main.bc", ErrInfo);
    WriteBitcodeToFile(TheModule, *out);
    out->flush();
}