#include "macosx/macosx_info.h"
#include "macosx/macosx_processor.h"

// Return the aggregate CPU utilization
float MacOSX_Processor::Utilization() {
  long currActiveJiffies = MacOSXInfo::Instance()->ActiveJiffies();
  long currIdleJiffies = MacOSXInfo::Instance()->IdleJiffies();

  return (computeCpuUtilization(currActiveJiffies, currIdleJiffies));
}