#ifndef MACOSX_SYSTEM_H
#define MACOSX_SYSTEM_H

#include "macosx_processor.h"
#include "process.h"
#include "system.h"

class MacOSX_System : public System {
 public:
  MacOSX_System();
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
  MacOSX_Processor macosx_cpu_;
  std::set<int> actualPids_{};
  std::set<int> newPids_{};

  // Comp Sort Function
  static bool wayToSort(shared_ptr<Process> p1, shared_ptr<Process> p2);
};

#endif
