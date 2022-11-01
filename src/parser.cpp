#include "parser.h"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace compiler;

void Parser::expect(TokenType t1, TokenType t2) {
  if (t1 != t2) {
    throw exception();
  }
}

vector<instruction> Parser::parseInstructions(vector<token> &tokens) {
  vector<instruction> instructions;
  instruction ni;
  op initOp = {-1, -1, -1, -1, -1};
  int i;

  for (token token : tokens) {
    if (token.type == Instruction) {
      i = 0;
      ni = {};
      ni.type = token.instructionType;
      ni.op1 = initOp;
      ni.op2 = initOp;
      ni.op3 = initOp;
      continue;
    }

    // Throw error if next token must be instruction
    if (i == -1) {
      throw exception();
    }

    switch (ni.type) {
      case NoOperation:
        i = -1;
        instructions.push_back(ni);
      case Load:
      case Store: {
        switch (i) {
          case 0: {
            expect(token.type, Register);
            i++;
            ni.op1.sr = token.value;
            break;
          }
          case 1: {
            expect(token.type, Arrow);
            i++;
            break;
          }
          case 2: {
            expect(token.type, Register);
            i = -1;

            if (ni.type == Store) {
              ni.op2.sr = token.value;
            }else  {
              ni.op3.sr = token.value;
            }

            instructions.push_back(ni);
            break;
          }
          default: {
            throw exception();
          }
        }

        break;
      }
      case LoadImmediate: {
        switch (i) {
          case 0: {
            expect(token.type, Constant);
            i++;
            ni.op1.vl = token.value;
            break;
          }
          case 1: {
            expect(token.type, Arrow);
            i++;
            break;
          }
          case 2: {
            expect(token.type, Register);
            i = -1;
            ni.op3.sr = token.value;
            instructions.push_back(ni);
            break;
          }
          default: {
            throw exception();
          }
        }

        break;
      }
      case Add:
      case Subtract:
      case Multiply:
      case LeftShift:
      case RightShift: {
        switch (i) {
          case 0: {
            expect(token.type, Register);
            ni.op1.sr = token.value;
            i++;
            break;
          }
          case 1: {
            expect(token.type, Comma);
            i++;
            break;
          }
          case 2: {
            expect(token.type, Register);
            ni.op2.sr = token.value;
            i++;
            break;
          }
          case 3: {
            expect(token.type, Arrow);
            i++;
            break;
          }
          case 4: {
            expect(token.type, Register);
            i = -1;
            ni.op3.sr = token.value;
            instructions.push_back(ni);
            break;
          }
          default: {
            throw exception();
          }
        }
        break;
      }
      case Output: {
        switch (i) {
          case 0: {
            expect(token.type, Constant);
            i = -1;
            ni.op1.vl = token.value;
            instructions.push_back(ni);
            break;
          }
          default: {
            throw exception();
          }
        }
        break;
      }
      default: {
        throw exception();
      }
    }
  }

  return instructions;
}
