#ifndef MACOSX_PROCESS_H
#define MACOSX_PROCESS_H

#include <string>

#include "process.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class MacOSX_Process : public Process {
 public:
  MacOSX_Process(const int& pid);
  ~MacOSX_Process() {};
  std::string Ram();  // TODO: See src/process.cpp
  long int UpTime();  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif