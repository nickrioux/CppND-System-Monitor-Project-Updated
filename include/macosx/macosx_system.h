#ifndef MACOSX_SYSTEM_H
#define MACOSX_SYSTEM_H

#include "process.h"
#include "system.h"
#include "macosx/macosx_processor.h"

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

 private:
  MacOSX_Processor macosx_cpu_;
  std::set<int> actualPids_{};
  std::set<int> newPids_{};
};

#endif
