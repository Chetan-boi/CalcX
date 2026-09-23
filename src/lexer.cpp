#include <lexer.hpp>

#include <cctype>
#include <iostream>
#include <vector>
#include <string>


std::vector<std::string> getInput() {
  std::cout << ">  ";
  std::string enteredEquation;
  if (std::getline(std::cin, enteredEquation)) {
    std::erase_if(enteredEquation, [](unsigned char c) { 
        return std::isspace(c); 
    });
  }
  std::vector<std::string> parsedEquation = parseCommand(enteredEquation);

  return parsedEquation;
}

std::vector<std::string> parseCommand(std::string enteredEquation) {

}
