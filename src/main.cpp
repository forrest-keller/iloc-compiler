#include <stdio.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "io.h"
#include "parser.h"
#include "register-allocator.h"
#include "scanner.h"

using namespace std;
using namespace compiler;

int main(int argc, char* argv[]) {
  int option;
  int numRegisters;
  bool shouldPrintHelp;
  bool verbose = false;
  IO* io = new IO();

  while ((option = getopt(argc, argv, "vk:h")) != -1) {
    switch (option) {
      case 'k':
        numRegisters = stoi(optarg);
        break;
      case 'v':
        verbose = true;
        break;
      case 'h':
        shouldPrintHelp = true;
        break;
    }
  }

  if (shouldPrintHelp) {
    io->showHelp();
    return 0;
  }

  if (!numRegisters) {
    throw exception();
  }

  char* fileName = argv[argc - 1];
  fstream file;
  file.open(fileName, ios::in);

  if (!file) {
    throw new exception();
  }

  Scanner* scanner = new Scanner();
  Parser* parser = new Parser();
  vector<token> tokens = scanner->scanTokenStream(file);
  vector<instruction> instructions = parser->parseInstructions(tokens);
  RegisterAllocator* ra =
      new RegisterAllocator(instructions, numRegisters, verbose);
  io->showIRTable(ra->allocatedBlock);
  io->showBlock(ra->allocatedBlock);
}