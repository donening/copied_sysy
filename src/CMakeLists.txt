cmake_minimum_required(VERSION 3.13)

project(calculator-c VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

add_executable(complier
    lexer.cpp
    parser.h
    parser.cpp
    pch.h
    tree.h
    tree.cpp
    type.h
    type.cpp
    main.cpp
)

add_custom_command(OUTPUT
    lexer.cpp
    PRE_BUILD
    COMMAND
    flex --noyywrap -o lexer.cpp lexer.l
    DEPENDS
    lexer.l
)

# 通过bison生成语法分析源代码
add_custom_command(OUTPUT
    parser.h parser.cpp
    COMMAND
    bison -o parser.cpp --defines=parser.h -v parser.y
    DEPENDS
    parser.y
)