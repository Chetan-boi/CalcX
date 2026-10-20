#pragma once

#include <string>
#include <vector>

double evaluate(const std::vector<std::string>& tokens, double lastAnswer = 0.0);

std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens);

int precedence(const std::string& op);
bool isRightAssociative(const std::string& op);
bool isOperator(const std::string& token);
bool isFunction(const std::string& token);
bool isNumber(const std::string& token);
bool isConstant(const std::string& token);
