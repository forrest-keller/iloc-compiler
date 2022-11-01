#include "scanner.h"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace compiler;

void Scanner::seekToken(fstream &fs) {
  while (fs && (c == '/' || c == ' ' || c == '\t' || c == '\n')) {
    fs.get(c);

    if (c == '/') {
      while (c != '\n') {
        fs.get(c);
      }
    }
  }
}

int Scanner::scanNumber(fstream &fs) {
  int val = 0;

  while (isdigit(c) && fs) {
    val *= 10;
    val += (int)c - 48;
    fs.get(c);
  }

  fs.unget();

  return val;
}

token Scanner::scanToken(fstream &fs) {
  switch (c) {
    case ',': {
      token newToken = {};
      newToken.type = Comma;
      return newToken;
    }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      token newToken = {};
      newToken.type = Constant;
      newToken.value = scanNumber(fs);
      return newToken;
    }
    case '=':
      fs.get(c);

      switch (c) {
        case '>': {
          token newToken = {};
          newToken.type = Arrow;
          return newToken;
        }
        default:
          throw exception();
      }
    case 's':
      fs.get(c);
      switch (c) {
        case 't':
          fs.get(c);
          switch (c) {
            case 'o':
              fs.get(c);
              switch (c) {
                case 'r':
                  fs.get(c);
                  switch (c) {
                    case 'e': {
                      token newToken = {};
                      newToken.type = Instruction;
                      newToken.instructionType = Store;
                      return newToken;
                    }
                    default:
                      throw exception();
                  }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        case 'u':
          fs.get(c);
          switch (c) {
            case 'b': {
              token newToken = {};
              newToken.type = Instruction;
              newToken.instructionType = Subtract;
              return newToken;
            }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    case 'l':
      fs.get(c);
      switch (c) {
        case 'o':
          fs.get(c);
          switch (c) {
            case 'a':
              fs.get(c);
              switch (c) {
                case 'd':
                  fs.get(c);
                  switch (c) {
                    case 'I': {
                      token newToken = {};
                      newToken.type = Instruction;
                      newToken.instructionType = LoadImmediate;
                      return newToken;
                    }
                    default: {
                      token newToken = {};
                      newToken.type = Instruction;
                      newToken.instructionType = Load;
                      return newToken;
                    }
                  }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        case 's':
          fs.get(c);
          switch (c) {
            case 'h':
              fs.get(c);
              switch (c) {
                case 'i':
                  fs.get(c);
                  switch (c) {
                    case 'f':
                      fs.get(c);
                      switch (c) {
                        case 't': {
                          token newToken = {};
                          newToken.type = Instruction;
                          newToken.instructionType = LeftShift;
                          return newToken;
                        }
                        default:
                          throw exception();
                      }
                    default:
                      throw exception();
                  }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    case 'r':
      fs.get(c);
      switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
          token newToken = {};
          newToken.type = Register;
          newToken.value = scanNumber(fs);
          return newToken;
        }
        case 's':
          fs.get(c);
          switch (c) {
            case 'h':
              fs.get(c);
              switch (c) {
                case 'i':
                  fs.get(c);
                  switch (c) {
                    case 'f':
                      fs.get(c);
                      switch (c) {
                        case 't': {
                          token newToken = {};
                          newToken.type = Instruction;
                          newToken.instructionType = RightShift;
                          return newToken;
                        }
                        default:
                          throw exception();
                      }
                    default:
                      throw exception();
                  }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        default: {
          throw exception();
        }
      }
    case 'm':
      fs.get(c);
      switch (c) {
        case 'u':
          fs.get(c);
          switch (c) {
            case 'l':
              fs.get(c);
              switch (c) {
                case 't': {
                  token newToken = {};
                  newToken.type = Instruction;
                  newToken.instructionType = Multiply;
                  return newToken;
                }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    case 'a':
      fs.get(c);
      switch (c) {
        case 'd':
          fs.get(c);
          switch (c) {
            case 'd': {
              token newToken = {};
              newToken.type = Instruction;
              newToken.instructionType = Add;
              return newToken;
            }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    case 'n':
      fs.get(c);
      switch (c) {
        case 'o':
          fs.get(c);
          switch (c) {
            case 'p': {
              token newToken = {};
              newToken.type = Instruction;
              newToken.instructionType = NoOperation;
              return newToken;
            }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    case 'o':
      fs.get(c);
      switch (c) {
        case 'u':
          fs.get(c);
          switch (c) {
            case 't':
              fs.get(c);
              switch (c) {
                case 'p':
                  fs.get(c);
                  switch (c) {
                    case 'u':
                      fs.get(c);
                      switch (c) {
                        case 't': {
                          token newToken = {};
                          newToken.type = Instruction;
                          newToken.instructionType = Output;
                          return newToken;
                        }
                        default:
                          throw exception();
                      }
                    default:
                      throw exception();
                  }
                default:
                  throw exception();
              }
            default:
              throw exception();
          }
        default:
          throw exception();
      }
    default:
      throw exception();
  }
}

vector<token> Scanner::scanTokenStream(fstream &fs) {
  vector<token> tokens;

  while (fs) {
    fs.get(c);
    seekToken(fs);

    if (fs) {
      tokens.push_back(scanToken(fs));
    }
  }

  return tokens;
}