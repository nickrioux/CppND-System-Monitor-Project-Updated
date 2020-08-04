#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// System Constructor
System::System() {
  // Initialize cpu_ TODO - Later create the right number of processors
}

bool sortDescend(const Process & a, const Process & b) { return !(a<b); }
void System::Update() {
    
    static int counter = 0;

    //Update System Info

    //Update Processes
    std::vector<int> pids{};
    std::vector<Process> processes{};

    pids = LinuxParser::Pids();

    if (pids.size() > 0) {
      for (int i = 0; i < (int)pids.size(); ++i) {
          processes.push_back(Process(pids[i]));
          processes.back().Update();
      }
    }

    //Sort Process
    std::sort(processes.begin(), processes.end(),sortDescend);

    processes_ = processes;

    counter++;
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }