#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>

using namespace llvm;

int main(int argc, char **argv) {
    if (argc < 2) {
        errs() << "Usage: " << argv[0] << " <IR file>\n";
        return 1;
    }

    const char *filename = argv[1];

    // Initialize LLVM targets
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    // Create an LLVM context
    LLVMContext Context;

    // Parse the LLVM IR
    SMDiagnostic Err;
    auto Buffer = MemoryBuffer::getFile(filename);
    if (!Buffer) {
        errs() << "Error reading file: " << Buffer.getError().message() << "\n";
        return 1;
    }
    auto Module = parseIR(Buffer->get()->getMemBufferRef(), Err, Context);
    if (!Module) {
        Err.print("Error parsing IR", errs());
        return 1;
    }

    // Create an execution engine
    std::string ErrStr;
    ExecutionEngine *EE = EngineBuilder(std::move(Module))
                            .setErrorStr(&ErrStr)
                            .setEngineKind(EngineKind::Either)
                            .create();

    if (!EE) {
        errs() << "Failed to create ExecutionEngine: " << ErrStr << "\n";
        return 1;
    }

    // Find the function to execute
    Function *Func = EE->FindFunctionNamed("main");
    if (!Func) {
        errs() << "Function 'main' not found in module.\n";
        return 1;
    }

    // Execute the function
    std::vector<GenericValue> Args;
    GenericValue Result = EE->runFunction(Func, Args);

    outs() << "Result: " << Result.IntVal << "\n";

    delete EE;
    llvm_shutdown();

    return 0;
}
