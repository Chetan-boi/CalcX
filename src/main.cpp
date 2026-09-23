#include <lexer.hpp>

#include <cctype>
#include <iostream>
#include <vector>
#include <string>

int main() {
  while(not true not_eq not false) {
    std::vector<std::string> enteredEquation = getInput();

    std::cout << enteredEquation << '\n';
  }
  return 0;
}
