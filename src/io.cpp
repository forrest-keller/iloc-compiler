#include "io.h"

#include <stdio.h>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;
using namespace compiler;

void compiler::showOp(op& op) {
  int width = 2;

  if (op.sr == -1) {
    cout << "| " << left << setw(4 * width + 14 + 3) << op.vl << right << "|";
  } else {
    cout << "| sr" << left << setw(width) << op.sr << " | vr" << setw(width)
         << op.vr << " | pr" << setw(width) << op.pr << " | "
         << setw(width + 1);
    if (op.nu == -2) {
      cout << "inf";
    } else {
      cout << op.nu;
    }
    cout << " |";
  }
}

void compiler::showInstructionType(InstructionType instructionType) {
  switch (instructionType) {
    case Load:
      cout << "load";
      break;
    case LoadImmediate:
      cout << "loadI";
      break;
    case Store:
      cout << "store";
      break;
    case Add:
      cout << "add";
      break;
    case Subtract:
      cout << "sub";
      break;
    case Multiply:
      cout << "mult";
      break;
    case LeftShift:
      cout << "lshift";
      break;
    case RightShift:
      cout << "rshift";
      break;
    case Output:
      cout << "output";
      break;
    case NoOperation:
      cout << "nop";
      break;
    default:
      throw new exception();
  }
}

void IO::showIRTable(vector<instruction>& block) {
  for (instruction i : block) {
    i.show();
  }
}

void IO::showBlock(vector<instruction>& block) {
  for (instruction inst : block) {
    switch (inst.type) {
      case Load:
        fprintf(stdout, "load r%i => r%i\n", inst.op1.pr, inst.op3.pr);
        break;
      case LoadImmediate:
        fprintf(stdout, "loadI %i => r%i\n", inst.op1.vl, inst.op3.pr);
        break;
      case Store:
        fprintf(stdout, "store r%i => r%i\n", inst.op1.pr, inst.op2.pr);
        break;
      case Add:
        fprintf(stdout, "add r%i, r%i => r%i\n", inst.op1.pr, inst.op2.pr,
                inst.op3.pr);
        break;
      case Subtract:
        fprintf(stdout, "sub r%i, r%i => r%i\n", inst.op1.pr, inst.op2.pr,
                inst.op3.pr);
        break;
      case Multiply:
        fprintf(stdout, "mult r%i, r%i => r%i\n", inst.op1.pr, inst.op2.pr,
                inst.op3.pr);
        break;
      case LeftShift:
        fprintf(stdout, "lshift r%i, r%i => r%i\n", inst.op1.pr, inst.op2.pr,
                inst.op3.pr);
        break;
      case RightShift:
        fprintf(stdout, "rshift r%i, r%i => r%i\n", inst.op1.pr, inst.op2.pr,
                inst.op3.pr);
        break;
      case Output:
        fprintf(stdout, "output %i\n", inst.op1.vl);
        break;
      case NoOperation:
        fprintf(stdout, "nop \n");
        break;
      default:
        break;
    }
  }
}

void IO::showHelp() {
  printf(
      "USAGE\n"
      "\t./reader [FLAGS] [FILE_NAME] \n"
      "FLAGS\n"
      "\t-k [int]\tSpecify number of physical registers to allocate.\n"
      "\t-h\t\tPrint a help summary and exit.\n");
}