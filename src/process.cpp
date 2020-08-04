#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Process Constructor
Process::Process(const int& pid) : pid_(pid), cpuUsage_(0.0) {
  // Retrive User Name
  user_ = LinuxParser::User(pid_);

  // Retrieve Command Name
  command_ = LinuxParser::Command(Pid());
}

void Process::Update() { computeCpuUtilisation(); }

// TODO: Return this process's ID
int Process::Pid() const { return (pid_); }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return (cpuUsage_); }

// Compute CPU Utilisation
void Process::computeCpuUtilisation() {

  float cpu_usage{0.0};

  long totalTime = LinuxParser::ActiveJiffies();
  long processTime = LinuxParser::ActiveJiffies(Pid());

  cpu_usage = (processTime - prev_ProcessTime)/(float)(totalTime - prev_TotalTime);

  prev_TotalTime = totalTime;
  prev_ProcessTime = processTime;

  cpuUsage_ = cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  if (CpuUtilization() < a.CpuUtilization()) {
    return true;
  } else {
    return false;
  }
}