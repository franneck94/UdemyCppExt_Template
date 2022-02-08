# Tooling

## Compiler

- Windows: g++, gdb, make via [Cygwin64](https://cygwin.com/install.html)
- Linux: g++, gdb, make via package manager (apt for Ubuntu for example)
- Mac: clang++, lldb, make via xcode-tools

## IDE: Visual Studio Code

- [Visual Studio Code](https://code.visualstudio.com/)

### VSCode Extensions

- Coding Tools Extension Pack (franneck94)
- C/C++ Extension Pack (franneck94)

## Clang Tools

We will use clang-format and clang-tidy.

Clang-format: Formatter tool for source code, will be installed by the Microsoft C/C++ extension  
Clang-tidy: Checks for issues in the code (similar to the compiler warnings)  

### Extra settings (for local .vscode/settings.json file)

```json
  "C_Cpp_Runner.warnings": [
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Woverflow",
    "-Wformat=2",
    "-Wsign-conversion",
    "-Wstrict-overflow=1",
    "-Wnull-dereference",
    "-Wdouble-promotion"
  ],
  "C_Cpp_Runner.compilerArgs": [
    "-pthread"
  ],
  "C_Cpp_Runner.includePaths": [
    "${workspaceFolder}/utils"
  ],
  "C_Cpp_Runner.linkerArgs": [
    "-lm"
  ],
  "C_Cpp_Runner.cStandard": "c99",
  "C_Cpp_Runner.cppStandard": "c++17",
  "C_Cpp_Runner.excludeSearch": [],
  "C_Cpp_Runner.enableWarnings": true,
  "C_Cpp_Runner.warningsAsError": false,
  "clang-tidy.compilerArgsBefore": [
    "-Iutils",
    "-D_CRT_SECURE_NO_WARNINGS"
  ],
  "clang-tidy.compilerArgs": [
    "-std=c++17"
  ]
```
