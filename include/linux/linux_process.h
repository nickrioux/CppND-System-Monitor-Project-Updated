#ifndef LINUX_PROCESS_H
#define LINUX_PROCESS_H

#include <string>

#include "process.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Linux_Process : public Process {
 public:
  Linux_Process(const int& pid);
  std::string Ram();  // TODO: See src/process.cpp
  long int UpTime();  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif