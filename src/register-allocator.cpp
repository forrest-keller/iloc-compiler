#include "register-allocator.h"

#include <stdio.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "io.h"

using namespace compiler;
using namespace std;

void instruction::show() {
  cout << "//" << left << " " << setw(6);
  showInstructionType(type);
  cout << "|";
  showOp(op1);
  showOp(op2);
  showOp(op3);
  cout << endl;
}

RegisterAllocator::RegisterAllocator(vector<instruction> _block,
                                     int _numRegisters, bool _verbose) {
  verbose = _verbose;
  memLoc = 32768;
  block = _block;
  numRegisters = _numRegisters;
  numInstructions = block.size();
  maxSR = getMaxSR();
  computeLastUse();
  maxVR = getMaxVR();
  allocateRegisters();
}

void RegisterAllocator::printTables() {
  // vector<int> VRtoPR, PRStack, VRtoNU, PRtoVR, VRtoMemLoc;
  cout << "// | VR | VRtoPR | VRtoNU | VRtoMem |" << endl;

  for (int i = 0; i < VRtoPR.size(); i++) {
    if (VRtoPR[i] != -1 || VRtoNU[i] != -1 || VRtoMemLoc[i] != -1) {
      cout << "// | " << i << " | " << VRtoPR[i] << " | " << VRtoNU[i] << " | "
           << VRtoMemLoc[i] << " | " << endl;
    }
  }

  cout << "// | PR | PRtoVR |" << endl;
  for (int i = 0; i < PRtoVR.size(); i++) {
    cout << "// | " << i << " | " << PRtoVR[i] << " |" << endl;
    if (PRtoVR[i] >= 0 && VRtoPR[PRtoVR[i]] != i) {
      cout << "inconsistant table" << endl;
    }
  }

  cout << "// stack: ";

  for (int i : PRStack) {
    cout << i << ", ";
  }

  cout << endl;
}

int RegisterAllocator::getMaxSR() {
  int x = -1;

  for (instruction i : block) {
    x = max(x, i.op1.sr);
    x = max(x, i.op2.sr);
    x = max(x, i.op3.sr);
  }

  return x;
}

int RegisterAllocator::getMaxVR() {
  int x = -1;

  for (instruction i : block) {
    x = max(x, i.op1.vr);
    x = max(x, i.op2.vr);
    x = max(x, i.op3.vr);
  }

  return x;
}

void RegisterAllocator::update(op& op, int index, vector<int>& SRtoVR,
                               vector<int>& lastUse) {
  if (op.sr != -1) {
    if (SRtoVR[op.sr] == -1) {
      SRtoVR[op.sr] = currVR++;
    }

    op.vr = SRtoVR[op.sr];
    op.nu = lastUse[op.sr];
    lastUse[op.sr] = index;
  }
}

void RegisterAllocator::computeLastUse() {
  vector<int> SRtoVR, lastUse;
  currVR = 0;

  for (int i = 0; i <= maxSR; i++) {
    SRtoVR.push_back(-1);
    lastUse.push_back(-2);
  }

  for (int i = numInstructions - 1; i >= 0; i--) {
    if (block[i].op3.sr != -1) {
      update(block[i].op3, i, SRtoVR, lastUse);

      SRtoVR[block[i].op3.sr] = -1;
      lastUse[block[i].op3.sr] = -2;
    }

    if (block[i].op1.sr != -1 && block[i].type != Output &&
        block[i].type != LoadImmediate) {
      update(block[i].op1, i, SRtoVR, lastUse);
    }

    if (block[i].op2.sr != -1) {
      update(block[i].op2, i, SRtoVR, lastUse);
    }
  }

  return;
}

int RegisterAllocator::getPR() {
  int PR;

  if (PRStack.empty()) {
    int maxNU, spillVR, spillPR;
    maxNU = -1;

    for (auto i = 1; i < PRtoVR.size(); i++) {
      if (PRtoVR[i] != -1 && VRtoNU[PRtoVR[i]] > maxNU) {
        spillVR = PRtoVR[i];
        spillPR = i;
        maxNU = VRtoNU[spillVR];
      }
    }

    instruction loadImmediateInstruction = {
        LoadImmediate,
        {memLoc, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -2, -1, 0, -1},
    };
    instruction storeInstruction = {
        Store,
        {-1, -2, spillVR, spillPR, -1},
        {-1, -2, -1, 0, -1},
        {-1, -1, -1, -1, -1},
    };

    allocatedBlock.push_back(loadImmediateInstruction);
    if (verbose) loadImmediateInstruction.show();
    allocatedBlock.push_back(storeInstruction);
    if (verbose) storeInstruction.show();
    VRtoMemLoc[spillVR] = memLoc;
    VRtoPR[spillVR] = -1;
    PRtoVR[spillPR] = -1;
    memLoc += 4;
    PR = spillPR;
  } else {
    PR = PRStack.back();
    PRStack.pop_back();
  }

  return PR;
}

void RegisterAllocator::freePR(op& op) {
  if (op.vr != -1) {
    if (op.nu == -2) {
      PRStack.push_back(op.pr);
      VRtoPR[op.vr] = -1;
      PRtoVR[op.pr] = -1;
      VRtoNU[op.vr] = -2;
    } else {
      VRtoNU[op.vr] = op.nu;
    }
  }
}

void RegisterAllocator::allocateRegister(op& op) {
  if (op.vr != -1) {
    if (VRtoPR[op.vr] == -1) {
      int PR = getPR();
      VRtoPR[op.vr] = PR;
      PRtoVR[PR] = op.vr;

      if (VRtoMemLoc[op.vr] != -1) {
        instruction loadImmediateInstruction = {
            LoadImmediate,
            {VRtoMemLoc[op.vr], -1, -1, -1, -1},
            {-1, -1, -1, -1, -1},
            {-1, -2, -1, 0, -1},
        };

        instruction loadInstruction = {
            Load,
            {-1, -2, -1, 0, -1},
            {-1, -1, -1, -1},
            {-1, -2, op.vr, PR, -1},
        };

        allocatedBlock.push_back(loadImmediateInstruction);
        if (verbose) loadImmediateInstruction.show();
        allocatedBlock.push_back(loadInstruction);
        if (verbose) loadInstruction.show();
      }
    }

    op.pr = VRtoPR[op.vr];
  }
}

void RegisterAllocator::allocateRegisters() {
  PRtoVR.push_back(-2);
  for (int i = 1; i < numRegisters; i++) {
    PRStack.push_back(i);
    PRtoVR.push_back(-1);
  }

  for (int i = 0; i <= maxVR; i++) {
    VRtoMemLoc.push_back(-1);
    VRtoNU.push_back(-1);
    VRtoPR.push_back(-1);
  }

  for (auto it = block.begin(); it != block.end(); it++) {
    allocateRegister(it->op1);
    allocateRegister(it->op2);
    freePR(it->op1);
    freePR(it->op2);

    if (it->op3.vr != -1) {
      int PR = getPR();
      VRtoPR[it->op3.vr] = PR;
      PRtoVR[PR] = it->op3.vr;
      it->op3.pr = PR;
      freePR(it->op3);
    }

    if (verbose) printTables();
    if (verbose) it->show();
    allocatedBlock.push_back(*it);
  }
}
