#include "gen.h"

LLVMContext TheContext;
Module TheModule("main", TheContext);
IRBuilder<> Builder(TheContext);

map<string, Function *> functions;
map<string, Value *> globalVariables;
map<string, vector<int>> globalArray;
map<Function *, map<string, Value*>> localVariables;
map<Function *, map<string, vector<int>>> localArray;

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

Value *getOffset(vector<int> array, vector<Value *> offset, IRBuilder<> funBuilder) {
    Value *o = ConstantInt::get(Type::getInt32Ty(TheContext), 0);
    int base = 1;
    for(int i = array.size() - 1;i >= 0;i--) {
        Value *tmp = funBuilder.CreateMul(ConstantInt::get(Type::getInt32Ty(TheContext), base), offset[i]);
        o = funBuilder.CreateAdd(tmp, o);
        base *= array[i];
    }
    return o;
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

    Function::arg_iterator iter = fun->arg_begin();
    if(argAST != nullptr) {
        for(auto arg : argAST->children) {
            iter->setName(arg->children[1]->name);
            localVariables[fun][arg->children[1]->name] = funBuilder.CreateAlloca(getType(arg->children[0]->name), nullptr, arg->children[1]->name);
            funBuilder.CreateStore(iter, localVariables[fun][arg->children[1]->name]);
            iter++;
        }
    }

    for(auto vars : blockAST->children[0]->children) genLocalVar(vars, funBuilder);
    for(auto stmt : blockAST->children[1]->children) genStmt(stmt, funBuilder);

    return fun;
}

BasicBlock *prevEnd = nullptr;

BasicBlock *genStmt(baseAST *ast, IRBuilder<> funBuilder) {
    if(ast->name == "ASSIGN") {
        genExp(ast, funBuilder);
    }
    else if(ast->name == "Input_Exp") {
        vector<Value *> args;
        string formatstr = "";
        for(int i = 1;i < ast->children[0]->name.length() - 1;i++) {
            if(ast->children[0]->name[i] == '\\' && ast->children[0]->name[i + 1] == 'n') {
                formatstr += '\n';
                i++;
            }
            else formatstr += ast->children[0]->name[i];
        }
        args.push_back(funBuilder.CreateGlobalStringPtr(formatstr));
        for(int i = 0;i < ast->children[1]->childCnt;i++) {
            auto arg = genExp(ast->children[1]->children[i], funBuilder);
            args.push_back(arg);
        }
        ArrayRef<Value *> argsRef(args);

        Value *res = funBuilder.CreateCall(funcScanf, argsRef);
        funBuilder.CreateStore(res, globalVariables["cinResult"]);
    }
    else if(ast->name == "Output_Exp") {
        vector<Value *> args;
        string formatstr = "";
        for(int i = 1;i < ast->children[0]->name.length() - 1;i++) {
            if(ast->children[0]->name[i] == '\\' && ast->children[0]->name[i + 1] == 'n') {
                formatstr += '\n';
                i++;
            }
            else formatstr += ast->children[0]->name[i];
        }
        args.push_back(funBuilder.CreateGlobalStringPtr(formatstr));
        if(ast->childCnt > 1) {
            for(int i = 0;i < ast->children[1]->childCnt;i++) {
                auto arg = genExp(ast->children[1]->children[i], funBuilder);
                args.push_back(arg);
            }
        }
        ArrayRef<Value *> argsRef(args);

        funBuilder.CreateCall(funcPrintf, argsRef);
    }
    else if(ast->name == "return") {
        if(ast->childCnt == 0) funBuilder.CreateRet(ConstantInt::get(Type::getInt32Ty(TheContext), 0));
        else funBuilder.CreateRet(genExp(ast->children[0], funBuilder));
    }
    else if(ast->name == "IF_Stmt") {
        BasicBlock *labelIfThen = BasicBlock::Create(TheContext, "if.then", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *labelIfEnd = BasicBlock::Create(TheContext, "if.end", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        funBuilder.CreateCondBr(genExp(ast->children[0], funBuilder), labelIfThen, labelIfEnd);
        Builder.SetInsertPoint(labelIfThen);
        BasicBlock *tmp = prevEnd;
        prevEnd = labelIfEnd;
        for(auto stmt : ast->children[1]->children[1]->children) genStmt(stmt, funBuilder);
        funBuilder.CreateBr(labelIfEnd);
        Builder.SetInsertPoint(labelIfEnd);
        prevEnd = tmp;
    }
    else if(ast->name == "If_Else_Stmt") {
        BasicBlock *labelIfThen = BasicBlock::Create(TheContext, "if.then", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *labelIfElse = BasicBlock::Create(TheContext, "if.else", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *labelIfEnd = BasicBlock::Create(TheContext, "if.end", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        funBuilder.CreateCondBr(genExp(ast->children[0], funBuilder), labelIfThen, labelIfElse);
        Builder.SetInsertPoint(labelIfThen);
        BasicBlock *tmp = prevEnd;
        prevEnd = labelIfElse;
        for(auto stmt : ast->children[1]->children[1]->children) genStmt(stmt, funBuilder);
        funBuilder.CreateBr(labelIfEnd);
        Builder.SetInsertPoint(labelIfElse);
        prevEnd = labelIfEnd;
        for(auto stmt : ast->children[2]->children[1]->children) genStmt(stmt, funBuilder);
        funBuilder.CreateBr(labelIfEnd);
        Builder.SetInsertPoint(labelIfEnd);
        prevEnd = tmp;
    }
    else if(ast->name == "While_Stmt") {
        BasicBlock *labelWhileCond = BasicBlock::Create(TheContext, "wihle.cond", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *labelWhileBody = BasicBlock::Create(TheContext, "while.body", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *labelWhileEnd = BasicBlock::Create(TheContext, "while.end", funBuilder.GetInsertBlock()->getParent(), prevEnd);
        BasicBlock *tmp = prevEnd;
        prevEnd = labelWhileEnd;
        funBuilder.CreateBr(labelWhileCond);
        Builder.SetInsertPoint(labelWhileCond);
        funBuilder.CreateCondBr(genExp(ast->children[0], funBuilder), labelWhileBody, labelWhileEnd);
        Builder.SetInsertPoint(labelWhileBody);
        for(auto stmt : ast->children[1]->children[1]->children) genStmt(stmt, funBuilder);
        funBuilder.CreateBr(labelWhileCond);
        Builder.SetInsertPoint(labelWhileEnd);
        prevEnd = tmp;
        return labelWhileEnd;
    }
    else if(ast->type == T_expr) {
        genExp(ast, funBuilder);
    }
    return nullptr;
}

Value *genExp(baseAST *ast, IRBuilder<> funBuilder) {
    Function *fun = funBuilder.GetInsertBlock()->getParent();

    if(ast->name == "constint") {
        return ConstantInt::get(Type::getInt32Ty(TheContext), ((constNode *) ast)->dvalue.integer);
    }
    else if(ast->name == "constfloat") {
        return ConstantFP::get(Type::getDoubleTy(TheContext), ((constNode *) ast)->dvalue.floatt);
    }
    else if(ast->type == T_var) {
        if(ast->childCnt == 0) {
            if(globalVariables[ast->name] != nullptr) return funBuilder.CreateLoad(globalVariables[ast->name]);
            else return funBuilder.CreateLoad(localVariables[fun][ast->name]);
        }
        else {
            if(globalVariables[ast->name] != nullptr) {
                vector<Value *> offsets;
                for(auto child : ast->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(globalArray[ast->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(globalVariables[ast->name], idxs);
                return funBuilder.CreateLoad(address);
            }
            else {
                vector<Value *> offsets;
                for(auto child : ast->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(localArray[fun][ast->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(localVariables[fun][ast->name], idxs);
                return funBuilder.CreateLoad(address);
            }
        }
    }
    else if(ast->name == "LOGICAND") {
        if(ast->children[0]->childCnt == 0) {
            if(globalVariables[ast->children[0]->name] != nullptr) return globalVariables[ast->children[0]->name];
            else return localVariables[fun][ast->children[0]->name];
        }
        else {
            if(globalVariables[ast->children[0]->name] != nullptr) {
                vector<Value *> offsets;
                for(auto child : ast->children[0]->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(globalArray[ast->children[0]->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(globalVariables[ast->children[0]->name], idxs);
                return address;
            }
            else {
                vector<Value *> offsets;
                for(auto child : ast->children[0]->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(localArray[fun][ast->children[0]->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(localVariables[fun][ast->children[0]->name], idxs);
                return address;
            }
        }
    }
    else if(ast->name == "ASSIGN") {
        if(ast->children[0]->childCnt == 0) {
            if(globalVariables[ast->children[0]->name] != nullptr) return funBuilder.CreateStore(genExp(ast->children[1], funBuilder), globalVariables[ast->children[0]->name]);
            else return funBuilder.CreateStore(genExp(ast->children[1], funBuilder), localVariables[fun][ast->children[0]->name]);
        }
        else {
            if(globalVariables[ast->children[0]->name] != nullptr) {
                vector<Value *> offsets;
                for(auto child : ast->children[0]->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(globalArray[ast->children[0]->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(globalVariables[ast->children[0]->name], idxs);
                return funBuilder.CreateStore(genExp(ast->children[1], funBuilder), address);
            }
            else {
                vector<Value *> offsets;
                for(auto child : ast->children[0]->children) offsets.push_back(genExp(child, funBuilder));
                Value *offset = getOffset(localArray[fun][ast->children[0]->name], offsets, funBuilder);
                Value *idxs[] = {ConstantInt::get(Type::getInt32Ty(TheContext), 0), offset};
                auto *address = funBuilder.CreateGEP(localVariables[fun][ast->children[0]->name], idxs);
                return funBuilder.CreateStore(genExp(ast->children[1], funBuilder), address);
            }
        }
    }
    else if(ast->type == T_expr) {
        if(ast->childCnt == 0) {
            vector<Value *> args;
            ArrayRef<Value *> argsRef(args);
            return funBuilder.CreateCall(functions[ast->name], argsRef);
        }
        else {
            vector<Value *> args;
            for(auto child : ast->children[0]->children) args.push_back(genExp(child, funBuilder));
            ArrayRef<Value *> argsRef(args);
            return funBuilder.CreateCall(functions[ast->name], argsRef);
        }
    }
    else if(ast->name == "MINUSDIGIT") {
        auto * child = genExp(ast->children[0], funBuilder);
        if(child->getType()->isDoubleTy()) return funBuilder.CreateFSub(ConstantFP::get(Type::getDoubleTy(TheContext), 0), child);
        else return funBuilder.CreateSub(ConstantInt::get(Type::getInt32Ty(TheContext), 0), child);
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
            if(type == "int") arrayType = ArrayType::get(Type::getInt32Ty(TheContext), len);
            else if(type == "float") arrayType = ArrayType::get(Type::getDoubleTy(TheContext), len);
            auto *constInit = llvm::ConstantAggregateZero::get(arrayType);
            Value *array = new GlobalVariable(TheModule, arrayType, false, GlobalValue::CommonLinkage, constInit, var->name);

            globalVariables[var->name] = array;
            globalArray[var->name] = arrayLen;
        }
    }

    TheModule.getOrInsertGlobal("cinResult", getType(type));
    auto coutRes = TheModule.getGlobalVariable("cinResult");
    coutRes->setLinkage(GlobalValue::CommonLinkage);
    coutRes->setInitializer(ConstantInt::get(Type::getInt32Ty(TheContext), 0));
    globalVariables["cinResult"] = coutRes;
}

void genLocalVar(baseAST *ast, IRBuilder<> funBuilder) {
    string type = ast->children[0]->name;
    vector<baseAST *> vars = ast->children[1]->children;
    Function *fun = funBuilder.GetInsertBlock()->getParent();
    for(auto var : vars) {
        if(var->childCnt == 0) {
            Value *lvar = funBuilder.CreateAlloca(getType(type), nullptr, var->name);
            localVariables[fun][var->name] = lvar;
        }
        else {
            vector<int> arrayLen;
            int len = 1;
            for(auto child : var->children) {
                arrayLen.push_back(((constNode *) child)->dvalue.integer);
                len *= ((constNode *) child)->dvalue.integer;
            }

            Type *arrayType;
            if(type == "int") arrayType = ArrayType::get(Type::getInt32Ty(TheContext), len);
            else if(type == "float") arrayType = ArrayType::get(Type::getDoubleTy(TheContext), len);
            auto *constInit = llvm::ConstantAggregateZero::get(arrayType);
            Value *array = funBuilder.CreateAlloca(arrayType, nullptr, var->name);

            localVariables[fun][var->name] = array;
            localArray[fun][var->name] = arrayLen;
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