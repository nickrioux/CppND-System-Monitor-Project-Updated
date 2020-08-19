#include <dirent.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux/linux_parser.h"

using std::stof;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string version;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    filestream.close();
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
std::set<int> LinuxParser::Pids() {
  std::set<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.insert(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal{0.0}, memFree{0.0}, memCached{0.0}, memBuffers{0.0};
  float memUsage{0.0};
  string key{""}, value{""};
  string line{""};

  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "MemTotal:") {
        memTotal = std::stof(value);
      } else if (key == "MemFree:") {
        memFree = std::stof(value);
      } else if (key == "Buffers:") {
        memBuffers = std::stof(value);
      } else if (key == "Cached:") {
        memCached = std::stof(value);
        break;
      }
    }

    memUsage = (memTotal - memFree - memBuffers - memCached) / memTotal;
  }
  return (memUsage);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string uptimeSystem, idleTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    filestream.close();
    std::istringstream linestream(line);
    linestream >> uptimeSystem >> idleTime;
  }
  return atol(uptimeSystem.c_str());
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  // return ActiveJiffies() + IdleJiffies();
  return UpTime() * sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> pidStats = CpuUtilization(pid);

  if (pidStats.size() > 0) {
    return (std::stol(pidStats[13]) +
            std::stol(pidStats[CPUTimesIndex::kUtimeIdx]) +
            std::stol(pidStats[CPUTimesIndex::kStimeIdx]) +
            std::stol(pidStats[CPUTimesIndex::kCutimeIdx]));
  } else {
    return (0);
  }
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<std::string> cpuStats = CpuUtilization();

  return (std::stol(cpuStats[kUser_]) + std::stol(cpuStats[kNice_]) +
          std::stol(cpuStats[kSystem_]) + std::stol(cpuStats[kIRQ_]) +
          std::stol(cpuStats[kSoftIRQ_]) + std::stol(cpuStats[kSteal_]));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpuStats = CpuUtilization();

  if (cpuStats.size() > 0) {
    return (std::stol(cpuStats[kIdle_]) + std::stol(cpuStats[kIOwait_]));
  } else {
    return (0);
  }
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuJiffies{};
  string value{""};
  string line{""};

  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    fileStream.close();
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu") {
        continue;
      }
      cpuJiffies.emplace_back(value);
    }
  }

  return (cpuJiffies);
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key;
  int value = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        break;
      }
    }
    filestream.close();
  }

  return (value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key;
  int value = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return (value);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string commandName;
  string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    commandName = line;
  }

  return (commandName);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  stringstream streamRam;
  string line;
  string key;
  long value = 0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        break;
      }
    }
  }

  // Convert to MB
  value /= 1024;

  // Format Ouput
  streamRam << value;

  return streamRam.str();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string Uid;
  string line;
  string key, value = "";

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }

  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string key, other, user, line = "";
  string Uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> other >> key;
      if (key == Uid) {
        filestream.close();
        return user;
      }
    }
    filestream.close();
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string sValue;
  long value = 0;
  vector<string> v;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    filestream.close();
    std::istringstream streamLine(line);
    for (int i = 0; i < 22; ++i) {
      streamLine >> sValue;
    }
    value = LinuxParser::UpTime() - std::stol(sValue) / sysconf(_SC_CLK_TCK);
  }

  return value;
}

vector<long> LinuxParser::CpuTimes(int pid) {
  string line;
  string sValue;
  vector<long> vValue{};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    filestream.close();
    std::istringstream streamLine(line);
    for (int i = 1; i <= CPUTimesIndex::kStarttimeIdx; ++i) {
      streamLine >> sValue;

      switch (i) {
        case CPUTimesIndex::kUtimeIdx:
        case CPUTimesIndex::kStimeIdx:
        case CPUTimesIndex::kCutimeIdx:
        case CPUTimesIndex::kCstimeIdx:
        case CPUTimesIndex::kStarttimeIdx:
          vValue.emplace_back(std::stol(sValue));
      }
    }
  }
  return vValue;
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  std::vector<string> values{};

  if (fileStream.is_open()) {
    string line;
    getline(fileStream, line);
    fileStream.close();

    std::istringstream linestream(line);
    string value;

    while (linestream >> value) {
      values.emplace_back(value);
    }
  }
  return values;
}