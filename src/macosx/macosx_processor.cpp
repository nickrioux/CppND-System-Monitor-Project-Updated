#include "macosx_processor.h"

#include "macosx_info.h"

// TODO: Return the aggregate CPU utilization
float MacOSX_Processor::Utilization() {
  long currActiveJiffies = MacOSXInfo::Instance()->ActiveJiffies();
  long currIdleJiffies = MacOSXInfo::Instance()->IdleJiffies();

  return (computeCpuUtilization(currActiveJiffies, currIdleJiffies));
}