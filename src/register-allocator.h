#pragma once

#include <tuple>
#include <vector>

#include "compiler.h"
#include "scanner.h"

using namespace std;
using namespace compiler;

namespace compiler {
class RegisterAllocator {
 public:
  vector<instruction> allocatedBlock;
  RegisterAllocator(vector<instruction> block, int numRegisters, bool verbose);
  void allocateRegisters();

 private:
  bool verbose;
  vector<instruction> block;
  vector<int> VRtoPR, PRStack, VRtoNU, PRtoVR, VRtoMemLoc;
  int memLoc, currVR, numRegisters, maxSR, maxVR, numInstructions;

  void update(op& op, int i, vector<int>& SRtoVR, vector<int>& lastUse);
  void computeLastUse();
  void freePR(op& op);
  void allocateRegister(op& op);
  void printTables();
  int getPR();
  int getMaxSR();
  int getMaxVR();
};
}  // namespace compiler