#pragma once

#include <tuple>
#include <vector>

#include "compiler.h"

using namespace std;
using namespace compiler;

namespace compiler {
class Scanner {
 public:
  token scanToken(fstream &fs);
  void seekToken(fstream &fs);
  int scanNumber(fstream &fs);
  vector<token> scanTokenStream(fstream &fs);

 private:
  char c;
};
}  // namespace compiler