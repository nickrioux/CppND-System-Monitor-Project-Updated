#include "macosx_system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "macosx_info.h"
#include "macosx_process.h"
#include "macosx_processor.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

MacOSX_System::MacOSX_System() : System(macosx_cpu_){};

// Sort Process Comparison Function
bool MacOSX_System::wayToSort(shared_ptr<Process> p1, shared_ptr<Process> p2) {
  return *p1 > *p2;
}

// TODO: Return a container composed of the system's processes
vector<shared_ptr<Process> >& MacOSX_System::Processes() {
  // Update Processes
  std::set<int> pids{};

  MacOSXInfo::Instance()->UpdateProcesses();

  pids = MacOSXInfo::Instance()->Pids();

  // Build a set of curr Pids
  set<int> currPids{};
  for (shared_ptr<Process> proc : GetProcesses()) {
    currPids.insert(proc->Pid());
  }

  // Update Processes with new PID and create set of new PIDs
  newPids_.clear();
  for (int pid : pids) {
    if (currPids.find(pid) == currPids.end()) {
      GetProcesses().push_back(make_shared<MacOSX_Process>(pid));
    }
    newPids_.insert(pid);
  }

  // Keep only the active processes comparing new PIDS with Processes
  for (auto i = GetProcesses().begin(); i != GetProcesses().end(); ++i) {
    if (newPids_.find((*i)->Pid()) == newPids_.end()) {
      (*i)->SetActive(false);
    } else {
      (*i)->SetActive(true);
      (*i)->ComputeCpuUtilization(
          MacOSXInfo::Instance()->ActiveJiffies((*i)->Pid()),
          MacOSXInfo::Instance()->Jiffies() * 10000000);
    }
  }

  GetProcesses().erase(
      std::remove_if(
          GetProcesses().begin(), GetProcesses().end(),
          [](const shared_ptr<Process> p) { return p->Active() == false; }),
      GetProcesses().end());

  // Sort Process
  std::sort(GetProcesses().begin(), GetProcesses().end(),
            MacOSX_System::wayToSort);

  return GetProcesses();
}

// TODO: Return the system's kernel identifier (string)
std::string MacOSX_System::Kernel() { return MacOSXInfo::Instance()->Kernel(); }

// TODO: Return the system's memory utilization
float MacOSX_System::MemoryUtilization() {
  return MacOSXInfo::Instance()->MemoryUtilization();
}

// TODO: Return the operating system name
std::string MacOSX_System::OperatingSystem() {
  return MacOSXInfo::Instance()->OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int MacOSX_System::RunningProcesses() {
  return MacOSXInfo::Instance()->RunningProcesses();
}

// TODO: Return the total number of processes on the system
int MacOSX_System::TotalProcesses() {
  return MacOSXInfo::Instance()->TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int MacOSX_System::UpTime() { return MacOSXInfo::Instance()->UpTime(); }
