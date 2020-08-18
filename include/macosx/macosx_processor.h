#include "processor.h"

#ifndef MACOSX_PROCESSOR_H
#define MACOSX_PROCESSOR_H

class MacOSX_Processor : public Processor {
 public:
  float Utilization();  // TODO: See src/macosx_processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif
