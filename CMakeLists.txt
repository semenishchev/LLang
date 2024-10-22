cmake_minimum_required(VERSION 3.16)
project(LLang)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_ARCH, "-m64")
set(TARGET_64 ON)
find_package(LLVM REQUIRED CONFIG)
include_directories("${LLVM_INCLUDE_DIRS}")
add_definitions("${LLVM_DEFINITION}")

include_directories(${PROJECT_SOURCE_DIR})

add_executable(LLang src/main.cpp src/lexer.cpp src/parser.cpp src/system.cpp)

add_compile_options(llvm-config --ldflags --libs)

llvm_map_components_to_libnames(llvm_libs
        Analysis
        Core
        ExecutionEngine
        InstCombine
        Object
        OrcJIT
        RuntimeDyld
        ScalarOpts
        Support
        native
        )
target_link_libraries(LLang ${llvm_libs})