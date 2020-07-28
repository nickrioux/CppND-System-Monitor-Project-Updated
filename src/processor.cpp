#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string cpuUsageString = LinuxParser::CpuUtilization().back();
  float cpuUsage = std::stof(cpuUsageString);
  return cpuUsage;
}