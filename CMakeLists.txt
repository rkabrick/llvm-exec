cmake_minimum_required(VERSION 3.24)
project(llvm-exec)
# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Define a variable for the LLVM installation directory
set(LLVM_DIR "" CACHE PATH "Path to the LLVM installation")

if(NOT LLVM_DIR)
  message(FATAL_ERROR "LLVM_DIR is not set. Please specify the path to the LLVM installation using -DLLVM_DIR=/path/to/llvm.")
endif()

# Find LLVM package
find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_DIR} NO_DEFAULT_PATH)

# Ensure the correct components are included
llvm_map_components_to_libnames(llvm_libs
    Core
    Support
    IRReader
    ExecutionEngine
    MCJIT
)
# Add target-specific components explicitly
# TODO: Make platform agnostic
set(target_specific_libs
    LLVMAArch64AsmParser
    LLVMAArch64CodeGen
    LLVMAArch64Desc
    LLVMAArch64Disassembler
    LLVMAArch64Info
    LLVMAArch64Utils
)
# Ensure that the LLVM_DIR is correctly pointing to the LLVM package
if(NOT LLVM_FOUND)
  message(FATAL_ERROR "Could not find LLVM")
endif()

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Include LLVM headers and link against LLVM libraries
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add the executable
add_executable(llvm-exec main.cpp)

target_link_libraries(llvm-exec ${llvm_libs} ${target_specific_libs})
