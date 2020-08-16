#ifndef LINUX_SYSTEM_H
#define LINUX_SYSTEM_H

#include "linux_processor.h"
#include "process.h"
#include "system.h"

class Linux_System : public System {
 public:
  Linux_System();
  Processor& Cpu();
  std::vector<shared_ptr<Process> >& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  Linux_Processor linux_cpu_;
  std::set<int> actualPids_{};
  std::set<int> newPids_{};

  // Comp Sort Function
  static bool wayToSort(shared_ptr<Process> p1, shared_ptr<Process> p2);
};

#endif