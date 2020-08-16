#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using std::make_shared;
using std::set;
using std::shared_ptr;
using std::vector;

class System {
 public:
  System(Processor&);
  Processor& Cpu();  // TODO: See src/system.cpp
  virtual std::vector<shared_ptr<Process> >&
  Processes();                            // TODO: See src/system.cpp
  virtual float MemoryUtilization();      // TODO: See src/system.cpp
  virtual long UpTime();                  // TODO: See src/system.cpp
  virtual int TotalProcesses();           // TODO: See src/system.cpp
  virtual int RunningProcesses();         // TODO: See src/system.cpp
  virtual std::string Kernel();           // TODO: See src/system.cpp
  virtual std::string OperatingSystem();  // TODO: See src/system.cpp

  vector<shared_ptr<Process> >& GetProcesses() { return processes_; }
  void SetProcesses(vector<shared_ptr<Process> >& vProcesses) {
    processes_ = vProcesses;
  }

  Processor& GetCpu() { return (cpu_); }

  // TODO: Define any necessary private members
 private:
  std::set<int> actualPids_;
  std::set<int> newPids_;

  Processor& cpu_;
  vector<shared_ptr<Process> > processes_;
};

#endif
