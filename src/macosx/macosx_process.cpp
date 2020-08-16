#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

#include "macosx_process.h"
#include "macosx_info.h"

// Process Constructor
MacOSX_Process::MacOSX_Process(const int& pid)
    : Process(pid, MacOSXInfo::Instance()->User(pid), MacOSXInfo::Instance()->Command(pid)) {
  // Contructor
}

// TODO: Return this process's memory utilization
string MacOSX_Process::Ram() { return (MacOSXInfo::Instance()->Ram(Pid())); }

// TODO: Return the age of this process (in seconds)
long int MacOSX_Process::UpTime() { return (MacOSXInfo::Instance()->UpTime(Pid())); }
