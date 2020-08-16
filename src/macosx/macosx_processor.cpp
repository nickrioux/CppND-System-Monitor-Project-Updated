#include "macosx_info.h"
#include "macosx_processor.h"

// TODO: Return the aggregate CPU utilization
float MacOSX_Processor::Utilization() {

  long currActiveJiffies = MacOSXInfo::Instance()->ActiveJiffies();
  long currIdleJiffies = MacOSXInfo::Instance()->IdleJiffies();

  long activeJiffies = currActiveJiffies - prevActiveJiffies;
  long idleJiffies = currIdleJiffies - prevIdleJiffies;

  long totalJiffies = activeJiffies + idleJiffies;

  float cpuUsage = static_cast<float>(activeJiffies) / totalJiffies;

  prevActiveJiffies = currActiveJiffies;
  prevIdleJiffies = currIdleJiffies;

  return cpuUsage;
}