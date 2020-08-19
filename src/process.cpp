#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Process Constructor
Process::Process(const int& pid, string user, string command)
    : pid_(pid), user_(user), command_(command), cpuUsage_(0.0) {
  // Contructor
}

void Process::SetActive(bool bFlag) { active_ = bFlag; }

bool Process::Active() const { return active_; }

// Return this process's ID
int Process::Pid() const { return (pid_); }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return (cpuUsage_); }

// Compute CPU Utilisation
void Process::ComputeCpuUtilization(long activeJiffies, long systemJiffies) {
  long totalTime = systemJiffies;
  long processTime = activeJiffies;

  cpuUsage_ = (processTime - getPrevProcessTime()) /
              (double)(totalTime - getPrevTotalTime());

  setPrevTotalTime(totalTime);
  setPrevProcessTime(processTime);
}

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

std::string Process::Ram() const { return ""; }
long Process::UpTime() const { return 0; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const a) const {
  return (CpuUtilization() < a.CpuUtilization());
}

bool Process::operator>(Process const a) const {
  return (CpuUtilization() > a.CpuUtilization());
}