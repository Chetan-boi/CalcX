# CalcX

A lightweight, high-performance command-line calculator inspired by qalc, built in C++.

## Features

- Arithmetic expression parsing and precedence handling
- Support for common mathematical functions and constants
- Fast interactive REPL mode and one-shot CLI evaluation
- Unit tracking and basic conversions

## Prerequisites

- C++20 compatible compiler (Clang 14+, GCC 12+, or Apple Clang)
- CMake 3.20 or newer

## Building from Source

1. Clone the repository:
   git clone https://github.com/username/CalcX.git
   cd CalcX

2. Generate the build files:
   cmake -B build -DCMAKE_BUILD_TYPE=Release

3. Compile the project:
   cmake --build build

## Usage

One-shot evaluation:
```sh
./build/CalcX "5 * (3 + 2)^2 - sqrt(16)"
```
