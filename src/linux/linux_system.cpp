#include "linux_system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "linux_process.h"
#include "linux_processor.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Linux_System::Linux_System() : System(linux_cpu_){};

// Sort Process Comparison Function
bool Linux_System::wayToSort(shared_ptr<Process> p1, shared_ptr<Process> p2) {
  return *p1 > *p2;
}

// TODO: Return a container composed of the system's processes
vector<shared_ptr<Process> >& Linux_System::Processes() {
  // Update Processes
  std::set<int> pids{};

  pids = LinuxParser::Pids();

  // Build a set of curr Pids
  set<int> currPids{};
  for (shared_ptr<Process> proc : GetProcesses()) {
    currPids.insert(proc->Pid());
  }

  // Update Processes with new PID and create set of new PIDs
  newPids_.clear();
  for (int pid : pids) {
    if (currPids.find(pid) == currPids.end()) {
      GetProcesses().push_back(make_shared<Linux_Process>(pid));
    }
    newPids_.insert(pid);
  }

  // Keep only the active processes comparing new PIDS with Processes
  for (auto i = GetProcesses().begin(); i != GetProcesses().end(); ++i) {
    if (newPids_.find((*i)->Pid()) == newPids_.end()) {
      (*i)->SetActive(false);
    } else {
      (*i)->SetActive(true);
      (*i)->ComputeCpuUtilization(LinuxParser::ActiveJiffies((*i)->Pid()),
                                  LinuxParser::Jiffies());
    }
  }

  GetProcesses().erase(
      std::remove_if(
          GetProcesses().begin(), GetProcesses().end(),
          [](const shared_ptr<Process> p) { return p->Active() == false; }),
      GetProcesses().end());

  // Sort Process
  std::sort(GetProcesses().begin(), GetProcesses().end(),
            Linux_System::wayToSort);

  return GetProcesses();
}

// TODO: Return the system's kernel identifier (string)
std::string Linux_System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float Linux_System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string Linux_System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int Linux_System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int Linux_System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int Linux_System::UpTime() { return LinuxParser::UpTime(); }