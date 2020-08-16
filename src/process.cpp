#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

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

// TODO: Return this process's ID
int Process::Pid() const { return (pid_); }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return (cpuUsage_); }

// Compute CPU Utilisation
void Process::ComputeCpuUtilization(long activeJiffies, long systemJiffies) {
  long totalTime = systemJiffies;
  long processTime = activeJiffies;

  cpuUsage_ =
      (processTime - prev_ProcessTime) / (double)(totalTime - prev_TotalTime);

  prev_TotalTime = totalTime;
  prev_ProcessTime = processTime;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return (string("")); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const a) const {
  if (CpuUtilization() < a.CpuUtilization()) {
    return true;
  } else {
    return false;
  }
}

bool Process::operator>(Process const a) const {
  if (CpuUtilization() > a.CpuUtilization()) {
    return true;
  } else {
    return false;
  }
}