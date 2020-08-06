#ifndef LINUX_SYSTEM_H
#define LINUX_SYSTEM_H

#include "linux_processor.h"
#include "process.h"
#include "system.h"

class Linux_System : public System {
 public:
  Linux_System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector< shared_ptr<Process> > & Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Linux_Processor linux_cpu_;
  std::set<int> actualPids_{};
  std::set<int> newPids_{};

  //Comp Sort Function
  static bool wayToSort(shared_ptr<Process> p1, shared_ptr<Process> p2);
};

#endif