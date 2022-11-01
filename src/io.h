#pragma once

#include <vector>

#include "compiler.h"

using namespace std;

namespace compiler {
class IO {
 public:
  void showHelp();
  void showInstruction(instruction&);
  void showIRTable(vector<instruction>& block);
  void showBlock(vector<instruction>& block);
};

}  // namespace compiler