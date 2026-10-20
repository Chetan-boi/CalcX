#include <evaluator.hpp>
#include <lexer.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string firstArg = argv[1];
        if (firstArg == "-h" || firstArg == "--help") {
            printHelp();
            return 0;
        }
        if (firstArg == "-v" || firstArg == "--version") {
            printVersion();
            return 0;
        }

        std::string expression;
        for (int i = 1; i < argc; ++i) {
            if (i > 1) expression += " ";
            expression += argv[i];
        }

        try {
            auto tokens = parseCommand(expression);
            double answer = evaluate(tokens);
            if (std::abs(answer) < 1e-15) {
                answer = 0.0;
            }
            std::cout << std::setprecision(12) << answer << '\n';
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    }

    std::cout << "\033[1mCalcX\033[0m - Lightweight CLI Calculator\n"
              << "Type an expression, 'help' for info, or 'exit' / 'quit' to quit.\n\n";

    bool isError = false;
    double lastAnswer = 0.0;

    while (true) {
        try {
            auto enteredEquation = getInput(isError);
            if (!enteredEquation.has_value()) {
                break; // EOF
            }

            if (enteredEquation->empty()) {
                continue;
            }

            double answer = evaluate(*enteredEquation, lastAnswer);
            if (std::abs(answer) < 1e-15) {
                answer = 0.0;
            }
            lastAnswer = answer;

            std::cout << std::setprecision(12) << answer << "\n\n";
            isError = false;
        } catch (const std::exception& e) {
            std::cout << "\033[31mError:\033[0m " << e.what() << "\n\n";
            isError = true;
        } catch (...) {
            std::cout << "\033[31mError:\033[0m Unknown error occurred\n\n";
            isError = true;
        }
    }

    return 0;
}
