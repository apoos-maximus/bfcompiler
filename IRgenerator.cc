#include "IRgenerator.h"

IRgenerator ::IRgenerator(progAST *bfast)
{
    this->AST = bfast;
}

IRgenerator ::~IRgenerator()
{
}

void emit_move_ptr(llvm::Value* ptr, int diff) {
  Builder.CreateStore(
      Builder.CreateInBoundsGEP(
        Builder.getInt8Ty(),
        Builder.CreateLoad(ptr),
        Builder.getInt32(diff)),
      ptr);
}

void emit_add(llvm::Value* ptr, int diff) {
  llvm::Value* tmp = Builder.CreateLoad(ptr);
  Builder.CreateStore(
      Builder.CreateAdd(
        Builder.CreateLoad(tmp),
        Builder.getInt8(diff)),
      tmp);
}

void emit_put(llvm::Value* ptr) {
  llvm::Function* funcPutChar = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("putchar",
        Builder.getInt32Ty(),
        Builder.getInt32Ty(),
        nullptr).getCallee());
  Builder.CreateCall(
      funcPutChar,
      Builder.CreateSExt(
        Builder.CreateLoad(Builder.CreateLoad(ptr)),
        Builder.getInt32Ty()));
}

void emit_get(llvm::Value* ptr) {
  llvm::Function* funcGetChar = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("getchar",
        Builder.getInt32Ty(),
        nullptr).getCallee());
  Builder.CreateStore(
      Builder.CreateTrunc(
        Builder.CreateCall(funcGetChar),
        Builder.getInt8Ty()),
      Builder.CreateLoad(ptr));
}

void emit_while_start(llvm::Function* func, llvm::Value* ptr, WhileBlock* while_block, int while_index) {
  while_block->cond_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_cond") + std::to_string(while_index), func);
  while_block->body_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_body") + std::to_string(while_index), func);
  while_block->end_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_end") + std::to_string(while_index), func);
  Builder.CreateBr(while_block->cond_block);
  Builder.SetInsertPoint(while_block->cond_block);
  Builder.CreateCondBr(
      Builder.CreateICmpNE(
        Builder.CreateLoad(Builder.CreateLoad(ptr)),
        Builder.getInt8(0)),
      while_block->body_block,
      while_block->end_block);
  Builder.SetInsertPoint(while_block->body_block);
}

void emit_while_end(WhileBlock* while_block) {
  Builder.CreateBr(while_block->cond_block);
  Builder.SetInsertPoint(while_block->end_block);
}

void IRgenerator ::codegen_prime(llvm ::Value *ptr, llvm::Function* mainFunc, progAST* treeptr){
    int while_index = 0;
  WhileBlock while_blocks[1000];
  WhileBlock* while_block_ptr = while_blocks;
  char c;

  progAST *tree_ptr = this->AST;

  while(tree_ptr != NULL) {
    c = tree_ptr->get_value();
    switch (c) {
      case '>': emit_move_ptr(ptr, 1); break;
      case '<': emit_move_ptr(ptr, -1); break;
      case '+': emit_add(ptr, 1); break;
      case '-': emit_add(ptr, -1); break;
      case '[': emit_while_start(mainFunc, ptr, while_block_ptr++, while_index++);
                codegen_prime(ptr, mainFunc, (dynamic_cast<loopAST*>(tree_ptr)->get_loop_content()));
                emit_while_end(while_block_ptr); break;
      case '.': emit_put(ptr); break;
      case ',': emit_get(ptr); break;
    }
  }
}

void IRgenerator ::codegen(){
      TheModule = make_unique<llvm::Module>("top", TheContext);
  llvm::Function* mainFunc = llvm::Function::Create(
      llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false),
      llvm::Function::ExternalLinkage, "main", TheModule.get());
  Builder.SetInsertPoint(llvm::BasicBlock::Create(TheContext, "", mainFunc));

  llvm::Value* data = Builder.CreateAlloca(Builder.getInt8PtrTy(), nullptr, "data");
  llvm::Value* ptr = Builder.CreateAlloca(Builder.getInt8PtrTy(), nullptr, "ptr");
  llvm::Function* funcCalloc = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("calloc",
        Builder.getInt8PtrTy(),
        Builder.getInt64Ty(), Builder.getInt64Ty(),
        nullptr).getCallee());
  llvm::Value* data_ptr = Builder.CreateCall(funcCalloc, {Builder.getInt64(30000), Builder.getInt64(1)});
  Builder.CreateStore(data_ptr, data);
  Builder.CreateStore(data_ptr, ptr);

  progAST* tr = this->AST;
  codegen_prime(ptr, mainFunc, tr );

    llvm::Function* funcFree = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("free",
        Builder.getVoidTy(),
        Builder.getInt8PtrTy(),
        nullptr).getCallee());
  Builder.CreateCall(funcFree, {Builder.CreateLoad(data)});

  Builder.CreateRet(Builder.getInt32(0));

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string TargetTriple = llvm::sys::getDefaultTargetTriple();

  std::string err;
  const llvm::Target* Target = llvm::TargetRegistry::lookupTarget(TargetTriple, err);
  if (!Target) {
    std::cerr << "Failed to lookup target " + TargetTriple + ": " + err;
    exit(1);
  }

  llvm::TargetOptions opt;
  llvm::TargetMachine* TheTargetMachine = Target->createTargetMachine(
      TargetTriple, "generic", "", opt, llvm::Optional<llvm::Reloc::Model>());

  TheModule->setTargetTriple(TargetTriple);
  TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  std::string Filename = "output.o";
  std::error_code err_code;
  llvm::raw_fd_ostream dest(Filename, err_code, llvm::sys::fs::F_None);
  if (err_code) {
    std::cerr << "Could not open file: " << err_code.message();
    exit(1);
  }

  llvm::legacy::PassManager pass;
  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile)) {
    std::cerr << "TheTargetMachine can't emit a file of this type\n";
    exit(1);
  }
  pass.run(*TheModule);
  dest.flush();
  std::cout << "Wrote " << Filename << "\n";
  
}