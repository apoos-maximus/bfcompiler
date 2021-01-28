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
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"

struct WhileBlock
{
  llvm::BasicBlock *cond_block;
  llvm::BasicBlock *body_block;
  llvm::BasicBlock *end_block;
};

class IRgenerator
{

public:
  progAST *AST;
  IRgenerator(progAST *bfast);
  ~IRgenerator();

  void spit_code(llvm::Function *mainF, llvm::Value *ptr, progAST *tr);
  llvm::Value *codegen_original(llvm::Function *mainF);
  void codegen();

  void emit_move_ptr(llvm::Value *ptr, int diff);
  void emit_add(llvm::Value *ptr, int diff);
  void emit_put(llvm::Value *ptr);
  void emit_get(llvm::Value *ptr);
  void emit_while(llvm::Function *func, llvm::Value *ptr, progAST *tr, WhileBlock *while_block, int while_index);
};