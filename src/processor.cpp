#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long currActiveJiffies = LinuxParser::ActiveJiffies();
  long currIdleJiffies = LinuxParser::IdleJiffies();


  float cpuUsage = ((currActiveJiffies - prevActiveJiffies)/(float)(currIdleJiffies-prevIdleJiffies));

  prevActiveJiffies = currActiveJiffies;
  prevIdleJiffies = currIdleJiffies;

  return cpuUsage;
}