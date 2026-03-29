# c-compiler

Pedagogical C-subset compiler built with ANTLR4 and C++ for the PLD (Projet Longue Duree) course at INSA Lyon (IF4).

The project includes:
- ANTLR parsing from [`compiler/ifcc.g4`](compiler/ifcc.g4)
- semantic analysis and scope/type checks in [`compiler/SymbolTableVisitor.cpp`](compiler/SymbolTableVisitor.cpp)
- IR construction in [`compiler/IRGenVisitor.cpp`](compiler/IRGenVisitor.cpp)
- code generation backends for x86_64, AArch64, and a textual IR dump

## Supported Subset

The current front-end handles:
- function definitions and forward declarations
- `int`, `double`, and `void`
- local variables, nested block scopes, and fixed-size local arrays
- assignments and expressions using `+`, `-`, `*`, `/`, `%`, unary `-`, unary `!`, comparisons, and bitwise `&`, `^`, `|`
- `if`/`else`, `while`, `switch`/`case`/`default`, `break`, and `return`
- recursion and calls with up to 6 arguments
- built-in `putchar()` and `getchar()`
- character literals such as `'A'`
- mixed `int`/`double` arithmetic with implicit numeric conversions

Semantic checks currently catch a large part of the intended subset, including undeclared variables, same-scope redeclarations, invalid array usage, incompatible function declarations, and several type errors.

## Current Limits

This is still a C subset, not a full ISO C compiler. In particular:
- no global variables
- no pointers, structs, strings, or `for` loops
- no logical `&&` or `||`
- arrays cannot be initialized in declarations
- arrays are local-only and are not supported as function parameters
- preprocessor lines are skipped rather than expanded
- the default CLI target is still `x86`, so native testing on Apple Silicon needs special handling

## Project Layout

```text
compiler/
  ifcc.g4                 ANTLR grammar
  main.cpp                CLI entry point and backend selection
  SymbolTableVisitor.*    semantic analysis
  IRGenVisitor.*          IR generation
  IR.h
  IRcore.cpp              shared IR structures
  IRx86.cpp               x86_64 backend
  IRarm.cpp               AArch64 backend
  Makefile
  config.example.mk
  config-IF501.mk
  config-wsl-2025.mk
antlr/
  jar/                    bundled ANTLR jar
  include/                ANTLR runtime headers
  lib/                    ANTLR runtime library
testfiles/                420 test programs grouped by feature
ifcc-test.py              GCC-vs-IFCC test harness
```

## Build

Prerequisites:
- a C++17 compiler
- Java
- Python 3
- ANTLR4 jar/runtime paths configured in `compiler/config.mk`

The repository already contains ANTLR assets under [`antlr/`](antlr), and the sample config files show different ways to point the build at them.

```bash
cd compiler
cp config.example.mk config.mk
# or start from config-IF501.mk / config-wsl-2025.mk

make
```

Build artifacts:
- `compiler/ifcc`: compiler executable
- `compiler/generated/`: ANTLR-generated parser/visitor files
- `compiler/build/`: object files and dependency files

## Usage

The compiler takes a C file and optionally a backend name:

```bash
./compiler/ifcc path/to/file.c
./compiler/ifcc path/to/file.c x86
./compiler/ifcc path/to/file.c arm
./compiler/ifcc path/to/file.c ir
```

Backends:
- `x86`: default output, emits x86_64 assembly
- `arm`: emits AArch64 assembly
- `ir`: prints the internal IR for debugging

### Native Apple Silicon Workflow

On Apple Silicon, the simplest path is to target `arm` and use the native toolchain:

```bash
./compiler/ifcc input.c arm > output.s
cc -o output output.s
./output
echo $?
```

### x86 Workflow

The default backend emits x86_64 assembly:

```bash
./compiler/ifcc input.c > output.s
```

You then need an x86_64-capable assembler/linker to build and run that output. On Apple Silicon, that usually means using Rosetta or another x86_64 toolchain.

### Parse Tree GUI

```bash
cd compiler
make gui FILE=path/to/file.c
```

## Testing

The repository ships with 420 `.c` tests covering arithmetic, syntax errors, function calls, function definitions, returns, `switch`, `while`, doubles, scoping, arrays, forward declarations, and demo programs.

Multi-file regression mode compares IFCC against GCC:

```bash
python3 ifcc-test.py testfiles
python3 ifcc-test.py -v testfiles
cd compiler && make test
```

Single-file mode mimics a subset of the GCC CLI:

```bash
python3 ifcc-test.py -S -o output.s input.c
python3 ifcc-test.py -c -o output.o input.c
python3 ifcc-test.py -o output input.c
```

The script automatically rebuilds `compiler/ifcc` when needed.

### Apple Silicon Test Caveat

[`ifcc-test.py`](ifcc-test.py) currently invokes `compiler/ifcc` without a backend argument, which means it exercises the default `x86` backend. On an ARM macOS host, the native assembler rejects that x86 assembly. If you want to use the bundled regression flow on Apple Silicon, run it from an x86_64 environment or adapt the script to request the `arm` backend.

## Clean

```bash
cd compiler
make clean
```
