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
  Processor& Cpu();
  virtual std::vector<shared_ptr<Process> >& Processes();
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual std::string Kernel() = 0;
  virtual std::string OperatingSystem() = 0;

  vector<shared_ptr<Process> >& GetProcesses() { return processes_; }
  void SetProcesses(vector<shared_ptr<Process> >& vProcesses) {
    processes_ = vProcesses;
  }

  Processor& GetCpu() { return (cpu_); }

 private:
  std::set<int> actualPids_;
  std::set<int> newPids_;

  Processor& cpu_;
  vector<shared_ptr<Process> > processes_;
};

#endif
