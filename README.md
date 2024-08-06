# LLVM IR Executor

Simplest form of an execution engine for LLVM IR (I think) written in c++.

# Building

```sh
mkdir build && cd build
cmake ../ -DLLVM_DIR=${PathToLLVM} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j
```

# Usage

```sh
./LLVMExecutor <path_to_ir_file>
```

# Note(s)

- If you get an error message `LLVM ERROR: Unsupported stack probing method` you have to make sure you are using the same version of clang included in your `DLLVM_DIR` argument passed during the compilation phase
