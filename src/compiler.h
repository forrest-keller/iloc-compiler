#pragma once

namespace compiler {
enum TokenType { Instruction, Constant, Arrow, Register, Comma, Void };
enum InstructionType {
  Load,
  LoadImmediate,
  Store,
  Add,
  Subtract,
  Multiply,
  LeftShift,
  RightShift,
  Output,
  NoOperation
};

struct token {
  TokenType type;
  InstructionType instructionType;
  int value;
};

struct op {
  int vl;  // Constant value
  int sr;  // Source register
  int vr;  // Virtual register
  int pr;  // Physical register
  int nu;  // Next use
};

struct instruction {
  void show();
  InstructionType type;
  op op1;
  op op2;
  op op3;
};

void showOp(op& op);
void showInstructionType(InstructionType instructionType);
}  // namespace compiler