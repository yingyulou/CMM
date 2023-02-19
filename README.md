# CMM

## Introduction

CMM (C Minus Minus) language compiler and VM.

## Dependencies

* C++17 compiler
* boost

## Installation

To compile CMM, make sure the location is in the ```CMM/src``` directory and simply run the following commands:

``` Bash
make
```

The executable file ```CMM``` will be created in ```CMM/bin/CMM```.

In addition, "make debug" is used to compile with the debug mode:

``` Bash
make debug
```

Use "make clean" to remove the installation:

``` Bash
make clean
```

## Getting Help

Command "CMM -h" or "CMM --help" is used to get the help information:

```
================================================================================

CMM
===

    CMM (C Minus Minus) Language Compiler And VM.

================================================================================

Usage:
  -h [ --help ]         show this help message and exit
  -i arg                Input CMM File Path
  -o arg (=a.out)       Output ASM File Path
  -r arg                Input ASM File Path (For Running)
```

## Sample files

There are two sample code files written by the CMM language in the ```CMM/test/testA.cmm``` and the ```CMM/test/testB.cmm```.

## CMM Language Grammar

Here is the CMM language grammar in EBNF format:

```
1. Program ::= Decl { Decl }

2. Decl ::= VarDecl
          | FuncDecl

3. VarDecl ::= Type ID [ '[' NUM ']' ] ';'

4. Type ::= int
          | void

5. FuncDecl ::= Type ID '(' [ ParamList ] ')' '{' LocalDecl StmtList '}'

6. ParamList ::= [ Param { ',' Param } ]

7. Param ::= Type ID [ '[' ']' ]

8. LocalDecl ::= { VarDecl }

9. StmtList ::= { Stmt }

10. Stmt ::= ExprStmt
           | IfStmt
           | WhileStmt
           | ReturnStmt

11. ExprStmt ::= [ Expr ] ';'

12. IfStmt ::= if '(' Expr ')' '{' StmtList '}' [ else '{' StmtList '}' ]

13. WhileStmt ::= while '(' Expr ')' '{' StmtList '}'

14. ReturnStmt ::= return [ Expr ] ';'

15. Expr ::= SimpleExpr
           | Var '=' Expr

16. Var ::= ID [ '[' Expr ']' ]

17. SimpleExpr ::= AddExpr [ RelOp AddExpr ]

18. RelOp ::= '<'
            | '<='
            | '>'
            | '>='
            | '=='
            | '!='

19. AddExpr ::= Term { AddOp Term }

20. AddOp ::= '+'
            | '-'

21. Term ::= Factor { MulOp Factor }

22. MulOp ::= '*'
            | '/'

23. Factor ::= '(' Expr ')'
             | NUM
             | Call
             | Var

24. Call ::= ID '(' [ ArgList ] ')'

25. ArgList ::= Expr { ',' Expr }
```

## VM

Here is the VM model of the CMM language:

```
+----+              +----+    +----+
| CS |              | SS |    | AX |
+----+              +----+    +----+
| .. |              | .. |
+----+    +----+    +----+    +----+
| .. | <= | IP |    | .. |    | BP |
+----+    +----+    +----+    +----+
  ..                  ..
```

CMM VM use only one SS to store all the data.

## Instruction Set

Here is the instruction set and the fake code of each instruction of the CMM language:

| Instruction | Fake Code                                         |
| :---------: | :-----------------------------------------------: |
| LDC N       | AX = N                                            |
| LD          | AX = SS[BP - AX]                                  |
| ALD         | AX = SS[AX]                                       |
| ST          | SS[BP - AX] = SS.TOP()                            |
| AST         | SS[AX] = SS.TOP()                                 |
| PUSH        | SS.PUSH(AX)                                       |
| POP         | SS.POP()                                          |
| JMP N       | IP += N                                           |
| JZ N        | if (AX == 0) IP += N                              |
| ADD         | AX = SS.TOP() + AX                                |
| SUB         | AX = SS.TOP() - AX                                |
| MUL         | AX = SS.TOP() * AX                                |
| DIV         | AX = SS.TOP() / AX                                |
| LT          | AX = SS.TOP() < AX                                |
| LE          | AX = SS.TOP() <= AX                               |
| GT          | AX = SS.TOP() > AX                                |
| GE          | AX = SS.TOP() >= AX                               |
| EQ          | AX = SS.TOP() == AX                               |
| NE          | AX = SS.TOP() != AX                               |
| IN          | scanf("%d", &AX)                                  |
| OUT         | printf("%d\n", AX)                                |
| ADDR N      | AX = SS.SIZE() - N                                |
| CALL N      | SS.PUSH(BP); BP = SS.SIZE(); SS.PUSH(IP); IP += N |
| RET         | IP = SS.POP(); BP = SS.POP()                      |

