# c-compiler

A C compiler built with ANTLR4 and C++ as part of the PLD (Projet Longue Duree) course at INSA Lyon (IF4).

Currently supports compiling minimal C programs (`int main() { return <const>; }`) down to x86 assembly.

## Project Structure

```
compiler/          # Compiler source code
  ifcc.g4          # ANTLR4 grammar definition
  main.cpp         # Entry point (parsing + code generation)
  CodeGenVisitor.h # AST visitor for x86 assembly generation
  CodeGenVisitor.cpp
  Makefile
  config.example.mk  # Example build configuration (copy to config.mk)
antlr/             # ANTLR4 runtime (headers + static library)
testfiles/         # Test programs (.c files)
ifcc-test.py       # Test runner (compares ifcc output against GCC)
```

## Prerequisites

- **g++** with C++17 support
- **Java** (to run the ANTLR4 jar)
- **ANTLR4 C++ runtime** (headers and library)

## Setup

1. Copy the example config and adjust paths for your system:

```bash
cd compiler
cp config.example.mk config.mk
# Edit config.mk to set ANTLRJAR, ANTLRINC, and ANTLRLIB paths
```

2. Build the compiler:

```bash
cd compiler
make
```

This produces the `ifcc` executable in `compiler/`.

## Usage

```bash
# Compile a C file to x86 assembly (output to stdout)
./compiler/ifcc path/to/file.c

# Save assembly, assemble, link, and run
./compiler/ifcc path/to/file.c > output.s
as -o output.o output.s
gcc -o output output.o
./output
echo $?  # print the return value
```

### macOS (Apple Silicon)

The compiler generates x86_64 assembly, so on Apple Silicon you need Rosetta to assemble and link:

```bash
./compiler/ifcc path/to/file.c > output.s
arch -x86_64 as -o output.o output.s
arch -x86_64 clang -o output output.o
./output
echo $?
```

> **Note:** The process exit status (`$?`) is an unsigned 8-bit value. For example, `return -1` will show as `255`. This is expected kernel behavior, not a bug.

### Parse tree

```bash
# View the parse tree in a GUI window
cd compiler && make gui FILE=path/to/file.c
```

## Testing

The test script compiles each test file with both GCC and ifcc, runs the resulting executables, and compares their output and exit codes.

```bash
python3 ifcc-test.py testfiles

# With verbose output
python3 ifcc-test.py -v testfiles

# Single-file mode
python3 ifcc-test.py -S -o output.s input.c
```

## Clean

```bash
cd compiler && make clean
```