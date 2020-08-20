#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux/linux_parser.h"
#include "linux/linux_process.h"

using std::string;
using std::to_string;
using std::vector;

// Process Constructor
Linux_Process::Linux_Process(const int& pid)
    : Process(pid, LinuxParser::User(pid), LinuxParser::Command(pid)) {
  // Contructor
}

// Return this process's memory utilization
string Linux_Process::Ram() const { return (LinuxParser::Ram(Pid())); }

// Return the age of this process (in seconds)
long int Linux_Process::UpTime() const { return (LinuxParser::UpTime(Pid())); }