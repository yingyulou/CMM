# CMM

## Introduction

CMM (C Minus Minus) language compiler and VM.

## Dependencies

* C++17 compiler
* boost

## Installation

To compile CMM, make sure the location is in the ```CMM/``` directory and simply run the following commands:

``` Bash
cd src
make
```

The executable file ```CMM``` will be created in ```CMM/bin/CMM```.

In addition, "make debug" is used to compile with the debug mode:

``` Bash
cd src
make debug
```

Use "make clean" to remove the installation:

``` Bash
cd src
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
  -c arg                Input CMM File Path
  -s arg                Output ASM File Path
  -o arg (=a.out)       Output Byte Code File Path
  -r arg                Input Byte Code File Path (For Running)
```

## Sample files

There are two sample code files written by the CMM language in the ```CMM/test/testA.cmm``` and the ```CMM/test/testB.cmm```.

## CMM Language Grammar

Here is the CMM language grammar in EBNF format:

```
1. Program ::= DeclList

2. DeclList ::= Decl { Decl }

3. Decl ::= VarDecl
          | FuncDecl

4. VarDecl ::= Type ID [ '[' NUM ']' ] ';'

5. Type ::= int
          | void

6. FuncDecl ::= Type ID '(' Params ')' CompoundStmt

7. Params ::= ParamList
            | void

8. ParamList ::= Param { ',' Param }

9. Param ::= Type ID [ '[' ']' ]

10. CompoundStmt ::= '{' LocalDecl StmtList '}'

11. LocalDecl ::= { VarDecl }

12. StmtList ::= { Stmt }

13. Stmt ::= ExprStmt
           | CompoundStmt
           | IfStmt
           | WhileStmt
           | ReturnStmt

14. ExprStmt ::= [ Expr ] ';'

15. IfStmt ::= if '(' Expr ')' Stmt [ else Stmt ]

16. WhileStmt ::= while '(' Expr ')' Stmt

17. ReturnStmt ::= return [ Expr ] ';'

18. Expr ::= Var '=' Expr
           | SimpleExpr

19. Var ::= ID [ '[' Expr ']' ]

20. SimpleExpr ::= AddExpr [ RelOp AddExpr ]

21. RelOp ::= '<'
            | '<='
            | '>'
            | '>='
            | '=='
            | '!='

22. AddExpr ::= Term { AddOp Term }

23. AddOp ::= '+'
            | '-'

24. Term ::= Factor { MulOp Factor }

25. MulOp ::= '*'
            | '/'

26. Factor ::= '(' Expr ')'
             | Var
             | Call
             | NUM

27. Call ::= ID '(' [ ArgList ] ')'

28. ArgList ::= Expr { ',' Expr }
```

## First(N) Set

Here is the First(N) set of the CMM language grammar:

```
1. Program ::= DeclList

    First(1): First(1) Of DeclList

2. DeclList ::= Decl { Decl }

    First(1): First(1) Of Decl

    First(1): !EOF ::= { Decl }

3. Decl ::= VarDecl
          | FuncDecl

    First(1): int | void

    First(2): ID

    First(3): '[' | ';' ::= VarDecl
              '('       ::= FuncDecl

4. VarDecl ::= Type ID [ '[' NUM ']' ] ';'

    First(1): First(1) Of Type

    First(1): '[' ::= [ '[' NUM ']' ]

5. Type ::= int
          | void

    First(1): int | void

6. FuncDecl ::= Type ID '(' Params ')' CompoundStmt

    First(1): First(1) Of Type

7. Params ::= ParamList
            | void

    First(1): int  ::= ParamList
              void ::= First(2)

    First(2): ID    ::= ParamList
              Other ::= void

8. ParamList ::= Param { ',' Param }

    First(1): First(1) Of Param

    First(1): ',' ::= { ',' Param }

9. Param ::= Type ID [ '[' ']' ]

    First(1): First(1) Of Type

    First(1): '[' ::= [ '[' ']' ]

10. CompoundStmt ::= '{' LocalDecl StmtList '}'

    First(1): '{'

11. LocalDecl ::= { VarDecl }

    First(1): int | void

12. StmtList ::= { Stmt }

    First(1): ';' | ID | '(' | NUM | '{' | if | while | return

13. Stmt ::= ExprStmt
           | CompoundStmt
           | IfStmt
           | WhileStmt
           | ReturnStmt

    First(1): ';' | ID | '(' | NUM ::= ExprStmt
               '{'                 ::= CompoundStmt
               if                  ::= IfStmt
               while               ::= WhileStmt
               return              ::= ReturnStmt

14. ExprStmt ::= [ Expr ] ';'

    First(1): ID | '(' | NUM ::= [ Expr ]

15. IfStmt ::= if '(' Expr ')' Stmt [ else Stmt ]

    First(1): if

16. WhileStmt ::= while '(' Expr ')' Stmt

    First(1): while

17. ReturnStmt ::= return [ Expr ] ';'

    First(1): return

    First(1): ID | '(' | NUM ::= [ Expr ]

18. Expr ::= Var '=' Expr
           | SimpleExpr

    First(1): '(' | NUM ::= SimpleExpr
              ID        ::= First(2)

    First(2): '('   ::= SimpleExpr
              Other ::= First(Var)

    First(Var): '='   ::= Var '=' Expr
                Other ::= SimpleExpr

19. Var ::= ID [ '[' Expr ']' ]

    First(1): ID

    First(1): '[' ::= [ '[' Expr ']' ]

20. SimpleExpr ::= AddExpr [ RelOp AddExpr ]

    First(1): First(1) Of AddExpr

    First(1): '<' | '<=' | '>' | '>=' | '==' | '!=' ::= [ RelOp AddExpr ]

21. RelOp ::= '<'
            | '<='
            | '>'
            | '>='
            | '=='
            | '!='

    First(1): '<' | '<=' | '>' | '>=' | '==' | '!='

22. AddExpr ::= Term { AddOp Term }

    First(1): First(1) Of Term

    First(1): '+' | '-' ::= { AddOp Term }

23. AddOp ::= '+'
            | '-'

    First(1): '+' | '-'

24. Term ::= Factor { MulOp Factor }

    First(1): First(1) Of Factor

    First(1): '*' | '/' ::= { MulOp Factor }

25. MulOp ::= *
            | /

    First(1): '*' | '/'

26. Factor ::= '(' Expr ')'
             | Var
             | Call
             | NUM

    First(1): '(' ::= '(' Expr ')'
              ID  ::= First(2)
              NUM ::= NUM

    First(2): '('   ::= Call
              Other ::= Var

27. Call ::= ID '(' [ ArgList ] ')'

    First(1): ID

    First(1): ID | '(' | NUM ::= [ ArgList ]

28. ArgList ::= Expr { ',' Expr }

    First(1): First(1) Of Expr

    First(1): ',' ::= { ',' Expr }
```

## VM

Here is the VM model of the CMM language:

```
+----+    +----+
| SS |    | CS |
+----+    +----+
| .. |    | .. |
+----+    +----+    +----+
| .. |    | .. | <= | IP |
+----+    +----+    +----+
  ..        ..
```

## Instruction Set

Here is the instruction set and the fake code of each instruction of the CMM language:

| Instruction | Fake Code                    |
| :---------: | :--------------------------: |
| LDC N       | SS[0] = N                    |
| LD          | SS[0] = SS[SS[1] - SS[0]]    |
| ABSLD       | SS[0] = SS[SS[0]]            |
| SAV         | SS[SS[1] - SS[0]] = SS.TOP() |
| ABSSAV      | SS[SS[0]] = SS.TOP()         |
| SAVSP       | SS[1] = SS.SIZE()            |
| PUSH        | SS.PUSH(SS[0])               |
| POP         | SS.POP()                     |
| PUSHBP      | SS.PUSH(SS[1])               |
| POPBP       | SS[1] = SS.POP()             |
| PUSHIP      | SS.PUSH(IP)                  |
| POPIP       | IP = SS.POP()                |
| PUSHSP      | SS.PUSH(SS.SIZE())           |
| JMP N       | IP += N                      |
| JZ N        | if (SS[0] == 0) IP += N      |
| ADD         | SS[0] = SS.TOP() + SS[0]     |
| SUB         | SS[0] = SS.TOP() - SS[0]     |
| MUL         | SS[0] = SS.TOP() * SS[0]     |
| DIV         | SS[0] = SS.TOP() / SS[0]     |
| LT          | SS[0] = SS.TOP() < SS[0]     |
| LE          | SS[0] = SS.TOP() <= SS[0]    |
| GT          | SS[0] = SS.TOP() > SS[0]     |
| GE          | SS[0] = SS.TOP() >= SS[0]    |
| EQ          | SS[0] = SS.TOP() == SS[0]    |
| NE          | SS[0] = SS.TOP() != SS[0]    |
| INPUT       | scanf("%d", &SS[0])          |
| OUTPUT      | printf("%d\n", SS[0])        |
| STOP        | exit(0)                      |

