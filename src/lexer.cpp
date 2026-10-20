#include <lexer.hpp>
#include <evaluator.hpp>
#include <version.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

static std::string to_lower(std::string str) {
  std::ranges::transform(str, str.begin(),
                         [](unsigned char c) { return std::tolower(c); });
  return str;
}


std::optional<std::vector<std::string>> getInput(bool isError) {
  if (isError) {
    std::cout << "\033[31m>  \033[0m";
  } else {
    std::cout << ">  ";
  }
  std::cout.flush();

  std::string enteredEquation;
  if (!std::getline(std::cin, enteredEquation)) {
    std::cout << '\n';
    return std::nullopt;
  }

  // Trim leading and trailing whitespace
  auto start = enteredEquation.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) {
    return std::vector<std::string>{};
  }
  auto end = enteredEquation.find_last_not_of(" \t\r\n");
  std::string trimmedCommand = enteredEquation.substr(start, end - start + 1);

  std::string loweredTrimmedCommand = to_lower(trimmedCommand);
  if (loweredTrimmedCommand == "exit" || loweredTrimmedCommand == "quit") {
    std::cout << "\nByee 👋\n";
    std::exit(0);
  }

  if (loweredTrimmedCommand == "help") {
    printHelp();
    return std::vector<std::string>{};
  }

  if (loweredTrimmedCommand == "clear") {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
    return std::vector<std::string>{};
  }

  return parseCommand(trimmedCommand);
}

std::vector<std::string> parseCommand(const std::string &enteredEquation) {
  std::vector<std::string> tokens;
  size_t i = 0;

  while (i < enteredEquation.length()) {
    unsigned char c = enteredEquation[i];

    // Skip whitespace
    if (std::isspace(c)) {
      ++i;
      continue;
    }

    // Numbers
    if (std::isdigit(c) || c == '.') {
      bool hasDecimal = (c == '.');
      std::string num;

      if (c == '.') {
        if (i + 1 >= enteredEquation.length() ||
            !std::isdigit(static_cast<unsigned char>(enteredEquation[i + 1]))) {
          throw std::invalid_argument(
              "Invalid number: decimal point must be followed by digits");
        }
      }

      while (i < enteredEquation.length()) {
        char cur = enteredEquation[i];

        if (std::isdigit(static_cast<unsigned char>(cur))) {
          num += enteredEquation[i++];
        } else if (cur == '.') {
          if (hasDecimal) {
            throw std::invalid_argument(
                "Invalid number: multiple decimal points in '" + num + ".'");
          }
          hasDecimal = true;
          num += enteredEquation[i++];
        } else if ((cur == 'e' || cur == 'E') && !num.empty()) {
          // Check if followed by optional +/- and digits
          size_t next = i + 1;
          if (next < enteredEquation.length() &&
              (enteredEquation[next] == '+' || enteredEquation[next] == '-')) {
            next++;
          }
          if (next < enteredEquation.length() &&
              std::isdigit(static_cast<unsigned char>(enteredEquation[next]))) {
            num += enteredEquation[i++]; // 'e' or 'E'
            if (i < enteredEquation.length() &&
                (enteredEquation[i] == '+' || enteredEquation[i] == '-')) {
              num += enteredEquation[i++];
            }
            while (i < enteredEquation.length() &&
                   std::isdigit(static_cast<unsigned char>(enteredEquation[i]))) {
              num += enteredEquation[i++];
            }
          }
          break;
        } else {
          break;
        }
      }

      tokens.push_back(num);
    }

    // Identifiers (functions, constants, etc.)
    else if (std::isalpha(c)) {
      std::string ident;

      while (i < enteredEquation.length() &&
             (std::isalpha(enteredEquation[i]) || std::isdigit(enteredEquation[i]))) {
        ident += static_cast<char>(std::tolower(enteredEquation[i]));
        ++i;
      }

      tokens.push_back(ident);
    }

    // Operators and delimiters
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
             c == '^' || c == '(' || c == ')' || c == ',') {
      tokens.push_back(std::string(1, c));
      ++i;
    }

    // Invalid character
    else {
      throw std::invalid_argument(std::string("Invalid character: '") +
                                  static_cast<char>(c) + "'");
    }
  }

  return tokens;
}

void printVersion() {
    std::cout << "\033[1m" << Config::APP_NAME << "\033[0m v" << Config::VERSION
              << " - Lightweight CLI Calculator\n"
              << "Homepage: " << Config::HOMEPAGE << "\n\n";

    std::cout << "\033[1mBuild Information:\033[0m\n"
              << "  Version:        " << Config::VERSION << " ("
              << Config::VERSION_MAJOR << '.' << Config::VERSION_MINOR << '.' << Config::VERSION_PATCH << ")\n"
              << "  Build Type:     " << Config::BUILD_TYPE << '\n'
              << "  Build Time:     " << Config::BUILD_DATE << " @ " << Config::BUILD_TIME << "\n\n";

    std::cout << "\033[1mNumerical Precision:\033[0m\n"
              << "  Precision:      IEEE 754 64-bit double (~15-17 significant decimal digits)\n"
              << "  Machine Epsilon: " << std::numeric_limits<double>::epsilon() << "\n\n";
}

void printHelp() {
  std::cout << "\n\033[1mCalcX - Lightweight CLI Calculator\033[0m\n\n"
            << "\033[1mCommands:\033[0m\n"
            << "  exit, quit       Exit the program\n"
            << "  help             Show this help menu\n"
            << "  clear            Clear the screen\n\n"
            << "\033[1mOperators:\033[0m\n"
            << "  +, -             Addition, subtraction, unary negation\n"
            << "  *, /, %          Multiplication, division, modulo\n"
            << "  ^                Exponentiation\n"
            << "  ( )              Parentheses for grouping\n\n"
            << "\033[1mFunctions:\033[0m\n"
            << "  sqrt, cbrt, abs, exp, ln, log, log2, log10\n"
            << "  sin, cos, tan, asin, acos, atan\n"
            << "  sinh, cosh, tanh\n"
            << "  floor, ceil, round\n"
            << "  min(a, b), max(a, b), pow(a, b)\n\n"
            << "\033[1mConstants:\033[0m\n"
            << "  pi               3.1415926535...\n"
            << "  e                2.7182818284...\n"
            << "  tau              6.2831853071...\n"
            << "  phi              1.6180339887...\n"
            << "  ans              Result of previous calculation\n\n"
            << "\033[1mExamples:\033[0m\n"
            << "  >  5 * (3 + 2)^2 - sqrt(16)\n"
            << "  121\n"
            << "  >  sin(pi / 2)\n"
            << "  1\n"
            << "  >  2pi\n"
            << "  6.28318530718\n"
            << "  >  ans * 2\n"
            << "  12.5663706144\n\n"

            << "You may also do one quick calculation\n"
            << "Example :-\n"
            << "→  CalcX 1+2\n"
            << "   3\n\n";
}
