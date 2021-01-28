#include "IRgenerator.h"

IRgenerator ::IRgenerator(progAST *bfast)
{
  this->AST = bfast;
}

IRgenerator ::~IRgenerator()
{
}

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;

int while_index = 0;
WhileBlock while_blocks[1000];
WhileBlock *while_block_ptr = while_blocks;

void IRgenerator ::emit_move_ptr(llvm::Value *ptr, int diff)
{
  Builder.CreateStore(
      Builder.CreateInBoundsGEP(
          Builder.getInt8Ty(),
          Builder.CreateLoad(ptr),
          Builder.getInt32(diff)),
      ptr);
}

void IRgenerator ::emit_add(llvm::Value *ptr, int diff)
{
  llvm::Value *tmp = Builder.CreateLoad(ptr);
  Builder.CreateStore(
      Builder.CreateAdd(
          Builder.CreateLoad(tmp),
          Builder.getInt8(diff)),
      tmp);
}

void IRgenerator ::emit_put(llvm::Value *ptr)
{
  llvm::Function *funcPutChar = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("putchar",
                                     Builder.getInt32Ty(),
                                     Builder.getInt32Ty(),
                                     nullptr)
          .getCallee());
  Builder.CreateCall(
      funcPutChar,
      Builder.CreateSExt(
          Builder.CreateLoad(Builder.CreateLoad(ptr)),
          Builder.getInt32Ty()));
}

void IRgenerator ::emit_get(llvm::Value *ptr)
{
  llvm::Function *funcGetChar = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("getchar",
                                     Builder.getInt32Ty(),
                                     nullptr)
          .getCallee());
  Builder.CreateStore(
      Builder.CreateTrunc(
          Builder.CreateCall(funcGetChar),
          Builder.getInt8Ty()),
      Builder.CreateLoad(ptr));
}

void IRgenerator ::emit_while(llvm::Function *func, llvm::Value *ptr, progAST *tr, WhileBlock *while_block, int while_index)
{
  WhileBlock *wb = while_block;
  int wi = while_index;
  wb->cond_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_cond") + std::to_string(wi), func);
  wb->body_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_body") + std::to_string(wi), func);
  wb->end_block = llvm::BasicBlock::Create(
      TheContext, std::string("while_end") + std::to_string(wi), func);
  Builder.CreateBr(wb->cond_block);
  Builder.SetInsertPoint(wb->cond_block);
  Builder.CreateCondBr(
      Builder.CreateICmpNE(
          Builder.CreateLoad(Builder.CreateLoad(ptr)),
          Builder.getInt8(0)),
      wb->body_block,
      wb->end_block);
  Builder.SetInsertPoint(wb->body_block);
  spit_code(func, ptr, tr->get_child());
  Builder.CreateBr(wb->cond_block);
  Builder.SetInsertPoint(wb->end_block);
  return;
}

void IRgenerator::spit_code(llvm ::Function *mainF, llvm ::Value *ptr, progAST *tr)
{
  if (tr == NULL)
    return;
  if (tr->get_value() == '[')
  {
    emit_while(mainF, ptr, tr, while_block_ptr++, while_index++);
    spit_code(mainF, ptr, tr->get_next());
  }
  else
  {
    if (tr->get_value() == '+')
    {
      emit_add(ptr, 1);
      spit_code(mainF, ptr, tr->get_next());
    }
    else if (tr->get_value() == '-')
    {
      emit_add(ptr, -1);
      spit_code(mainF, ptr, tr->get_next());
    }
    else if (tr->get_value() == '>')
    {
      emit_move_ptr(ptr, 1);
      spit_code(mainF, ptr, tr->get_next());
    }
    else if (tr->get_value() == '<')
    {
      emit_move_ptr(ptr, -1);
      spit_code(mainF, ptr, tr->get_next());
    }
    else if (tr->get_value() == '.')
    {
      emit_put(ptr);
      spit_code(mainF, ptr, tr->get_next());
    }
    else if (tr->get_value() == ',')
    {
      emit_get(ptr);
      spit_code(mainF, ptr, tr->get_next());
    }
  }
}

llvm::Value *IRgenerator ::codegen_original(llvm ::Function *mainF)
{
  llvm::Value *data = Builder.CreateAlloca(Builder.getInt8PtrTy(), nullptr, "data");
  llvm::Value *ptr = Builder.CreateAlloca(Builder.getInt8PtrTy(), nullptr, "ptr");
  llvm::Function *funcCalloc = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("calloc",
                                     Builder.getInt8PtrTy(),
                                     Builder.getInt64Ty(), Builder.getInt64Ty(),
                                     nullptr)
          .getCallee());
  llvm::Value *data_ptr = Builder.CreateCall(funcCalloc, {Builder.getInt64(30000), Builder.getInt64(1)});
  Builder.CreateStore(data_ptr, data);
  Builder.CreateStore(data_ptr, ptr);
  progAST *tree = this->AST;
  spit_code(mainF, ptr, tree);
  llvm::Function *funcFree = llvm::cast<llvm::Function>(
      TheModule->getOrInsertFunction("free",
                                     Builder.getVoidTy(),
                                     Builder.getInt8PtrTy(),
                                     nullptr)
          .getCallee());
  Builder.CreateCall(funcFree, {Builder.CreateLoad(data)});
  Builder.CreateRet(Builder.getInt32(0));

  // funcCalloc->print(llvm::errs());.
  // funcFree->print(llvm::errs());s
  mainF->print(llvm::errs());

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string TargetTriple = llvm::sys::getDefaultTargetTriple();

  std::string err;
  const llvm::Target *Target = llvm::TargetRegistry::lookupTarget(TargetTriple, err);
  if (!Target)
  {
    std::cerr << "Failed to lookup target " + TargetTriple + ": " + err;
    return NULL;
  }

  llvm::TargetOptions opt;
  llvm::TargetMachine *TheTargetMachine = Target->createTargetMachine(
      TargetTriple, "generic", "", opt, llvm::Optional<llvm::Reloc::Model>());

  TheModule->setTargetTriple(TargetTriple);
  TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  std::string Filename = "output.o";
  std::error_code err_code;
  llvm::raw_fd_ostream dest(Filename, err_code, llvm::sys::fs::F_None);
  if (err_code)
  {
    std::cerr << "Could not open file: " << err_code.message();
    return NULL;
  }

  llvm::legacy::PassManager pass;
  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile))
  {
    std::cerr << "TheTargetMachine can't emit a file of this type\n";
    return NULL;
  }
  
  pass.run(*TheModule);
  dest.flush();
  std::cout << "Wrote " << Filename << "\n";
  return NULL;
}

void IRgenerator ::codegen()
{
  TheModule = std::make_unique<llvm::Module>("bfcompiler", TheContext);
  llvm::Function *mainF = TheModule->getFunction("main");
  if (!mainF)
  {
    mainF = llvm ::Function ::Create(llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false),
                                     llvm::Function::ExternalLinkage,
                                     "main",
                                     TheModule.get());
  }
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", mainF);
  Builder.SetInsertPoint(BB);
  llvm ::Value *mainFBody = codegen_original(mainF);
  Builder.CreateRet(mainFBody);
}