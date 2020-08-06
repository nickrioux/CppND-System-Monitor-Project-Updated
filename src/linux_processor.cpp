#include "linux_processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Linux_Processor::Utilization() {
  long currActiveJiffies = LinuxParser::ActiveJiffies();
  long currIdleJiffies = LinuxParser::IdleJiffies();

  long activeJiffies = currActiveJiffies - prevActiveJiffies;
  long idleJiffies = currIdleJiffies - prevIdleJiffies;

  long totalJiffies = activeJiffies + idleJiffies;

  float cpuUsage = static_cast<float>(activeJiffies) / totalJiffies;

  prevActiveJiffies = currActiveJiffies;
  prevIdleJiffies = currIdleJiffies;

  return cpuUsage;
}