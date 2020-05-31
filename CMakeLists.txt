cmake_minimum_required(VERSION 3.16)
project(pl0)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-DLEX -DGRAMMAR -DVM)
add_compile_options(-DPRINT_GRAMMAR_TREE)

include_directories(src src/grammar src/lex src/symbol src/vm)

aux_source_directory(src/grammar GRAMMAR_SRCS)
aux_source_directory(src/lex LEX_SRCS)
aux_source_directory(src/symbol SYMBOL_SRCS)
aux_source_directory(src/vm VM_SRCS)
aux_source_directory(src MAIN_SRCS)

add_executable(pl0 ${SYMBOL_SRCS} ${LEX_SRCS} ${GRAMMAR_SRCS} ${VM_SRCS} ${MAIN_SRCS})