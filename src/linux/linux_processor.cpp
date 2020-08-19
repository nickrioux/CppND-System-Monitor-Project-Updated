#include "linux/linux_parser.h"
#include "linux/linux_processor.h"

// Return the aggregate CPU utilization
float Linux_Processor::Utilization() {
  long currActiveJiffies = LinuxParser::ActiveJiffies();
  long currIdleJiffies = LinuxParser::IdleJiffies();

  return (computeCpuUtilization(currActiveJiffies, currIdleJiffies));
}