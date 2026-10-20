#include <evaluator.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <numbers>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

static double evaluateRPN(const std::vector<std::string>& rpn, double lastAnswer) {
    if (rpn.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    std::stack<double> valStack;

    for (const std::string& token : rpn) {
        if (isNumber(token)) {
            try {
                valStack.push(std::stod(token));
            } catch (...) {
                throw std::invalid_argument("Invalid number format: '" + token + "'");
            }
        } else if (token == "pi") {
            valStack.push(std::numbers::pi_v<double>);
        } else if (token == "e") {
            valStack.push(std::numbers::e_v<double>);
        } else if (token == "tau") {
            valStack.push(2.0 * std::numbers::pi_v<double>);
        } else if (token == "phi") {
            valStack.push(std::numbers::phi_v<double>);
        } else if (token == "ans") {
            valStack.push(lastAnswer);
        } else if (token == "u-") {
            if (valStack.empty()) {
                throw std::invalid_argument("Missing operand for unary minus");
            }
            double val = valStack.top();
            valStack.pop();
            valStack.push(-val);
        } else if (token == "u+") {
            if (valStack.empty()) {
                throw std::invalid_argument("Missing operand for unary plus");
            }
            // Value is unchanged
        } else if (isOperator(token)) {
            if (valStack.size() < 2) {
                throw std::invalid_argument("Missing operand for operator '" + token + "'");
            }
            double b = valStack.top();
            valStack.pop();
            double a = valStack.top();
            valStack.pop();

            if (token == "+") {
                valStack.push(a + b);
            } else if (token == "-") {
                valStack.push(a - b);
            } else if (token == "*") {
                valStack.push(a * b);
            } else if (token == "/") {
                if (b == 0.0) {
                    throw std::runtime_error("Division by zero");
                }
                valStack.push(a / b);
            } else if (token == "%") {
                if (b == 0.0) {
                    throw std::runtime_error("Division by zero in modulo");
                }
                valStack.push(std::fmod(a, b));
            } else if (token == "^") {
                valStack.push(std::pow(a, b));
            } else {
                throw std::invalid_argument("Unknown operator: '" + token + "'");
            }
        } else if (isFunction(token)) {
            if (token == "pow" || token == "min" || token == "max") {
                if (valStack.size() < 2) {
                    throw std::invalid_argument("Function '" + token + "' requires two arguments");
                }
                double b = valStack.top();
                valStack.pop();
                double a = valStack.top();
                valStack.pop();

                if (token == "pow") valStack.push(std::pow(a, b));
                else if (token == "min") valStack.push(std::min(a, b));
                else if (token == "max") valStack.push(std::max(a, b));
            } else {
                if (valStack.empty()) {
                    throw std::invalid_argument("Missing argument for function '" + token + "'");
                }
                double a = valStack.top();
                valStack.pop();

                if (token == "sqrt") {
                    if (a < 0.0) throw std::domain_error("Square root of negative number");
                    valStack.push(std::sqrt(a));
                } else if (token == "cbrt") {
                    valStack.push(std::cbrt(a));
                } else if (token == "sin") {
                    valStack.push(std::sin(a));
                } else if (token == "cos") {
                    valStack.push(std::cos(a));
                } else if (token == "tan") {
                    valStack.push(std::tan(a));
                } else if (token == "asin") {
                    if (a < -1.0 || a > 1.0) throw std::domain_error("asin argument must be between -1 and 1");
                    valStack.push(std::asin(a));
                } else if (token == "acos") {
                    if (a < -1.0 || a > 1.0) throw std::domain_error("acos argument must be between -1 and 1");
                    valStack.push(std::acos(a));
                } else if (token == "atan") {
                    valStack.push(std::atan(a));
                } else if (token == "sinh") {
                    valStack.push(std::sinh(a));
                } else if (token == "cosh") {
                    valStack.push(std::cosh(a));
                } else if (token == "tanh") {
                    valStack.push(std::tanh(a));
                } else if (token == "exp") {
                    valStack.push(std::exp(a));
                } else if (token == "ln") {
                    if (a <= 0.0) throw std::domain_error("ln argument must be positive");
                    valStack.push(std::log(a));
                } else if (token == "log" || token == "log10") {
                    if (a <= 0.0) throw std::domain_error("log argument must be positive");
                    valStack.push(std::log10(a));
                } else if (token == "log2") {
                    if (a <= 0.0) throw std::domain_error("log2 argument must be positive");
                    valStack.push(std::log2(a));
                } else if (token == "abs") {
                    valStack.push(std::abs(a));
                } else if (token == "floor") {
                    valStack.push(std::floor(a));
                } else if (token == "ceil") {
                    valStack.push(std::ceil(a));
                } else if (token == "round") {
                    valStack.push(std::round(a));
                } else {
                    throw std::invalid_argument("Unknown function: '" + token + "'");
                }
            }
        } else {
            throw std::invalid_argument("Unknown token: '" + token + "'");
        }
    }

    if (valStack.size() != 1) {
        throw std::invalid_argument("Invalid expression: redundant operands or missing operator");
    }

    double result = valStack.top();
    if (std::abs(result) < 1e-15) {
        result = 0.0;
    }
    return result;
}

double evaluate(const std::vector<std::string>& tokens, double lastAnswer) {
    auto rpn = shuntingYard(tokens);
    return evaluateRPN(rpn, lastAnswer);
}

std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    std::vector<std::string> output;
    std::stack<std::string> operators;
    bool expectUnary = true;

    for (const std::string& token : tokens) {
        // If number or constant, add to output
        if (isNumber(token) || isConstant(token)) {
            output.emplace_back(token);
            expectUnary = false;
        }

        // If left parenthesis
        else if (token == "(") {
            operators.push(token);
            expectUnary = true;
        }

        // If argument separator (comma)
        else if (token == ",") {
            while (!operators.empty() && operators.top() != "(") {
                output.emplace_back(operators.top());
                operators.pop();
            }
            if (operators.empty()) {
                throw std::invalid_argument("Misplaced comma ',' outside parentheses");
            }
            expectUnary = true;
        }

        // If right parenthesis
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.emplace_back(operators.top());
                operators.pop();
            }

            if (operators.empty()) {
                throw std::invalid_argument("Mismatched parentheses: missing '('");
            }

            // Remove the opening '('
            operators.pop();

            // If top of operator stack is a function, pop it to output
            if (!operators.empty() && isFunction(operators.top())) {
                output.emplace_back(operators.top());
                operators.pop();
            }

            expectUnary = false;
        }

        // If function, push to stack
        else if (isFunction(token)) {
            operators.push(token);
            expectUnary = true;
        }

        // If operator
        else if (isOperator(token)) {
            std::string op = token;
            if (expectUnary) {
                if (op == "-") {
                    op = "u-";
                } else if (op == "+") {
                    op = "u+";
                } else {
                    throw std::invalid_argument("Unexpected operator: '" + op + "'");
                }
            }

            if (op == "u-" || op == "u+") {
                operators.push(op);
            } else {
                while (!operators.empty() && isOperator(operators.top())) {
                    std::string top = operators.top();

                    bool higherPrecedence = precedence(top) > precedence(op);
                    bool equalPrecedence = (precedence(top) == precedence(op));
                    bool leftAssociative = !isRightAssociative(op);

                    if (higherPrecedence || (equalPrecedence && leftAssociative)) {
                        output.emplace_back(top);
                        operators.pop();
                    } else {
                        break;
                    }
                }
                operators.push(op);
            }

            expectUnary = true;
        }

        else {
            throw std::invalid_argument("Unknown token: '" + token + "'");
        }
    }

    if (expectUnary) {
        throw std::invalid_argument("Incomplete expression: expected operand at end");
    }

    // Pop remaining operators into output
    while (!operators.empty()) {
        if (operators.top() == "(" || operators.top() == ")") {
            throw std::invalid_argument("Mismatched parentheses: unclosed '('");
        }

        output.emplace_back(operators.top());
        operators.pop();
    }

    return output;
}

int precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "u-" || op == "u+") return 3;
    if (op == "^") return 4;
    return 0;
}

bool isRightAssociative(const std::string& op) {
    return op == "^" || op == "u-" || op == "u+";
}

bool isOperator(const std::string& token) {
    return token == "+" || token == "-" ||
           token == "*" || token == "/" ||
           token == "%" || token == "^" ||
           token == "u-" || token == "u+";
}

bool isFunction(const std::string& token) {
    return token == "sin" || token == "cos" || token == "tan" ||
           token == "asin" || token == "acos" || token == "atan" ||
           token == "sinh" || token == "cosh" || token == "tanh" ||
           token == "sqrt" || token == "cbrt" ||
           token == "log" || token == "log10" || token == "log2" || token == "ln" ||
           token == "exp" || token == "abs" ||
           token == "floor" || token == "ceil" || token == "round" ||
           token == "pow" || token == "min" || token == "max";
}

bool isNumber(const std::string& token) {
    if (token.empty()) return false;

    size_t i = 0;
    if (token[0] == '+' || token[0] == '-') {
        i++;
    }
    if (i >= token.size()) return false;

    bool hasDigit = false;
    bool hasDecimal = false;

    while (i < token.size()) {
        char c = token[i];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            hasDigit = true;
            i++;
        } else if (c == '.' && !hasDecimal) {
            hasDecimal = true;
            i++;
        } else if ((c == 'e' || c == 'E') && hasDigit) {
            i++;
            if (i < token.size() && (token[i] == '+' || token[i] == '-')) {
                i++;
            }
            bool expDigit = false;
            while (i < token.size() && std::isdigit(static_cast<unsigned char>(token[i]))) {
                expDigit = true;
                i++;
            }
            return expDigit && (i == token.size());
        } else {
            return false;
        }
    }

    return hasDigit;
}

bool isConstant(const std::string& token) {
    return token == "pi" || token == "e" ||
           token == "tau" || token == "phi" ||
           token == "ans";
}
