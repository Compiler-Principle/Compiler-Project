#include "gen.h"

static std::unique_ptr<LLVMContext> TheContext = std::make_unique<LLVMContext>();
static std::unique_ptr<Module> TheModule = std::make_unique<Module>("compiler", *TheContext);
static std::unique_ptr<IRBuilder<>> Builder = std::make_unique<IRBuilder<>>(*TheContext);

Type *getType(string type) {
    if(type == "int") {
        return Type::getInt32Ty(*TheContext);
    }
    else if(type == "float") {
        return Type::getFloatTy(*TheContext);
    }
    else if(type == "void") {
        return Type::getVoidTy(*TheContext);
    }
    return Type::getVoidTy(*TheContext);
}

Function *genFun(baseAST *ast) {
    baseAST *retAST, *argAST;
    
    retAST = ast->children[0];
    if(ast->childCnt == 3) argAST = ast->children[1];
    else argAST = nullptr;

    Type *returnType = getType(retAST->name); // 返回类型

    vector<Type *> funArgs; // 参数
    if(argAST != nullptr) {
        for(auto arg : argAST->children) {
            funArgs.push_back(getType(arg->children[0]->name));
            cout << arg->children[0]->name << endl;
        }
    }

    FunctionType *funcType;
    funcType = FunctionType::get(returnType, funArgs, false);
    Function *fun = Function::Create(funcType, Function::ExternalLinkage, Twine(ast->name), *TheModule);
    return nullptr;
}

void genCode(baseAST *ast) {
    baseAST *globalVar = ast->children[0];
    baseAST *funs = ast->children[1];
    for(auto child : funs->children) {
        auto fun = genFun(child);
    }
}

void genBC() {
    std::error_code ErrInfo;
    raw_ostream *out = new raw_fd_ostream("main.bc", ErrInfo);
    WriteBitcodeToFile(*TheModule, *out);
    out->flush();
}