#include "processor.h"

#ifndef MACOSX_PROCESSOR_H
#define MACOSX_PROCESSOR_H

class MacOSX_Processor : public Processor {
 public:
  float Utilization();

 private:
};

#endif
