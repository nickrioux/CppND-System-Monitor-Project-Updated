#include "linux_processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Linux_Processor::Utilization() {
  long currActiveJiffies = LinuxParser::ActiveJiffies();
  long currIdleJiffies = LinuxParser::IdleJiffies();

  return (computeCpuUtilization(currActiveJiffies, currIdleJiffies));
}