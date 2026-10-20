# CalcX

A lightweight, high-performance command-line calculator inspired by qalc, built in C++.

## Features

- Arithmetic expression parsing and precedence handling
- Support for common mathematical functions and constants
- Fast interactive REPL mode and one-shot CLI evaluation
- Unit tracking and basic conversions
- Build-time version metadata utilizing `version.h` (freshly baked like "cakes" by CMake from `version.h.in`)

## Prerequisites

- C++20 compatible compiler (Clang 14+, GCC 12+, or Apple Clang)
- CMake 3.20 or newer

## Building from Source

1. Clone the repository:
   ```sh
   git clone https://github.com/Chetan-boi/CalcX.git
   cd CalcX
   ```

2. Generate the build files:
   ```sh
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```
   > During configuration, CMake acts like an oven—taking the `include/version.h.in` recipe and baking fresh `version.h` "cakes" into `build/include/` with exact build timestamps, type, and version numbers.

3. Compile the project:
   ```sh
   cmake --build build
   ```

## Usage

### One-Shot evaluation

Run quick calculations directly from your terminal:

```sh
CalcX "5 * (3 + 2)^2 - sqrt(16)"
# 121

CalcX 1+2
# 3
```

### Version & Info

Check verbose version details, build metadata, and floating-point precision specs utilizing `version.h`:

```sh
CalcX -v
# or
CalcX --version
```

---

### Interactive REPL Mode

Launch the interactive prompt by running the binary with no arguments:

```sh
CalcX
```


#### Commands 


| Command | Action |
| :--- | :--- |
| `help` | Show the help menu |
| `clear` | Clear the terminal screen |
| `exit`, `quit` | Exit the program |



#### Functions

* **Arithmetic / Utility:** `sqrt`, `cbrt`, `abs`, `exp`, `ln`, `log`, `log2`, `log10`, `floor`, `ceil`, `round`
* **Multi-Argument:** `min(a, b)`, `max(a, b)`, `pow(a, b)`
* **Trigonometry:** `sin`, `cos`, `tan`, `asin`, `acos`, `atan`
* **Hyperbolic:** `sinh`, `cosh`, `tanh`

#### Constants

| Constant | Value / Description |
| :--- | :--- |
| `pi` | `3.1415926535...` |
| `e` | `2.7182818284...` |
| `tau` | `6.2831853071...` |
| `phi` | `1.6180339887...` |
| `ans` | Result of previous calculation |

---

### Examples

```text
→ 5 * (3 + 2)^2 - sqrt(16)
121

→ sin(pi / 2)
1

→ 2pi
6.28318530718

→ ans * 2
12.5663706144
