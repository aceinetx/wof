# Codebase documentation
This document is here just to walk you through where everything is located and it's responsibility in the codebase

## ```Compiler``` class
Located in src/compiler/\*.cc (include/compiler/\*.hpp)

Generates IR code and parses expressions

The most important class in the entire codebase

## ```Log``` singleton
Located in src/log.cc (include/log.hpp)

Prints logs to the console with a prefix, contains ```INFO```, ```WARN```, ```ERROR``` macros

## ```SExprObject``` class
Located in src/sexpr.cc (include/sexpr.hpp)

Contains information for s-expressions

## ```SExpr``` class
Located in src/sexpr.cc (include/sexpr.hpp)

Parses S-expressions and outputs ```SExprObject```. Used in ```Compiler``` class

## ```Lexer``` class
Located in src/lexer.cc (include/lexer.hpp)

Tokenizes code step by step, called in ```SExpr::next```

## ```wtype``` class
Located in src/compiler/wtype.cc (include/compiler/wtype.hpp)

Contains information for a LLVM type and pointer information (if needed). Can be safely converted to ```llvm::Type*``` with the overloaded operator