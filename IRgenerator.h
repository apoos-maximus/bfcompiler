#pragma once
#include "bfnode.h"
#include "llvm/ADT/Optional.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"


struct WhileBlock {
  llvm::BasicBlock* cond_block;
  llvm::BasicBlock* body_block;
  llvm::BasicBlock* end_block;
};

class IRgenerator {
    
    public:
        progAST *AST;
        IRgenerator(progAST* bfast);
        ~IRgenerator();

        void codegen();
        void codegen_prime(llvm:: Value* ptr, llvm::Function* mainFunc, progAST* treeptr);

        void emit_move_ptr(llvm::Value* ptr, int diff);
        void emit_add(llvm::Value* ptr, int diff);
        void emit_put(llvm::Value* ptr);
        void emit_get(llvm::Value* ptr);
        void emit_while_start(llvm::Function* func, llvm::Value* ptr, WhileBlock* while_block, int while_index);
        void emit_while_end(WhileBlock* while_block);
};