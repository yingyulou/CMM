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
  -h [ --help ]          Show this help message and exit
  --input-file-path arg  Input cmm file path
  --output-file-path arg Output asm file path
  --asm-file-path arg    Input asm file path for running
```

## Sample files

There are two sample code files written by the CMM language in the ```CMM/test/testA.c``` and the ```CMM/test/testB.c```.

## CMM Language Grammar

Here is the CMM language grammar in EBNF format:

```
1. Program ::= Decl { Decl }

2. Decl ::= VarDecl
          | FuncDecl

3. VarDecl ::= Type ID [ '[' Number ']' ] ';'

4. Type ::= int
          | void

5. FuncDecl ::= Type ID '(' ParamList ')' '{' LocalDecl StmtList '}'

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
             | Number
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
| ldc n       | ax = n                                            |
| ld          | ax = ss[bp - ax]                                  |
| ald         | ax = ss[ax]                                       |
| st          | ss[bp - ax] = ss.top()                            |
| ast         | ss[ax] = ss.top()                                 |
| push        | ss.push(ax)                                       |
| pop         | ss.pop()                                          |
| jmp n       | ip += n                                           |
| jz n        | if (ax == 0) ip += n                              |
| add         | ax = ss.top() + ax                                |
| sub         | ax = ss.top() - ax                                |
| mul         | ax = ss.top() * ax                                |
| div         | ax = ss.top() / ax                                |
| lt          | ax = ss.top() < ax                                |
| le          | ax = ss.top() <= ax                               |
| gt          | ax = ss.top() > ax                                |
| ge          | ax = ss.top() >= ax                               |
| eq          | ax = ss.top() == ax                               |
| ne          | ax = ss.top() != ax                               |
| in          | scanf("%d", &ax)                                  |
| out         | printf("%d\n", ax)                                |
| lea n       | ax = ss.size() - n                                |
| call n      | ss.push(bp); bp = ss.size(); ss.push(ip); ip += n |
| ret         | ip = ss.pop(); bp = ss.pop()                      |

