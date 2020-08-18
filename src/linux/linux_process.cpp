#include "linux_process.h"

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
Linux_Process::Linux_Process(const int& pid)
    : Process(pid, LinuxParser::User(pid), LinuxParser::Command(pid)) {
  // Contructor
}

// TODO: Return this process's memory utilization
string Linux_Process::Ram() const { return (LinuxParser::Ram(Pid())); }

// TODO: Return the age of this process (in seconds)
long int Linux_Process::UpTime() const { return (LinuxParser::UpTime(Pid())); }