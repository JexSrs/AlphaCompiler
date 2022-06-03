# --------------------------------------------
# THIS FILE IS ONLY FOR CLION ERROR CHECKING.
# PLEASE CONSULT THE MAKEFILE IN THE SAME DIRECTORY.
# --------------------------------------------

cmake_minimum_required(VERSION 3.21)
project(project C)

set(CMAKE_C_STANDARD 23)

FILE(GLOB SOURCES Compiler/*.c Compiler/bison/*.c Compiler/lex/*.c
        Compiler/intermediate/*.c Compiler/targetCode/*.c
        VirtualMachine/execFuncs/*.c VirtualMachine/*.c VirtualMachine/tools/*.c
        build/*.c)
FILE(GLOB HEADERS Compiler/*.h Compiler/bison/*.h Compiler/lex/*.h
        Compiler/intermediate/*.h Compiler/targetCode/*.h
        VirtualMachine/execFuncs/*.h VirtualMachine/*.h VirtualMachine/tools/*.h
        build/*.h)
add_executable(project ${SOURCES} ${HEADERS})
