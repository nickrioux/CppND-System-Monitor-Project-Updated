#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

#include "macosx/macosx_info.h"
#include "macosx/macosx_process.h"

// Process Constructor
MacOSX_Process::MacOSX_Process(const int& pid)
    : Process(pid, MacOSXInfo::Instance()->User(pid),
              MacOSXInfo::Instance()->Command(pid)) {
  // Contructor
}

// Return this process's memory utilization
string MacOSX_Process::Ram() const {
  return (MacOSXInfo::Instance()->Ram(Pid()));
}

// Return the age of this process (in seconds)
long int MacOSX_Process::UpTime() const {
  return (MacOSXInfo::Instance()->UpTime(Pid()));
}
