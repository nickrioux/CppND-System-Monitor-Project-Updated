#include "processor.h"

// Return the aggregate CPU utilization
float Processor::computeCpuUtilization(long currActiveJiffies,
                                       long currIdleJiffies) {
  long activeJiffies = currActiveJiffies - getPrevActiveJiffies();
  long idleJiffies = currIdleJiffies - getPrevIdleJiffies();

  long totalJiffies = activeJiffies + idleJiffies;

  float cpuUsage = static_cast<float>(activeJiffies) / totalJiffies;

  setPrevActiveJiffies(currActiveJiffies);
  setPrevIdleJiffies(currIdleJiffies);

  return (cpuUsage);
}
