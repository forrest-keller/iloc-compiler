#pragma once

#include <tuple>
#include <vector>

#include "compiler.h"

using namespace std;
using namespace compiler;

namespace compiler {
class Parser {
 public:
  vector<instruction> parseInstructions(vector<token> &tokens);

 private:
  void expect(TokenType, TokenType);
};
}  // namespace compiler