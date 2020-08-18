#include "processor.h"

#ifndef LINUX_PROCESSOR_H
#define LINUX_PROCESSOR_H

class Linux_Processor : public Processor {
 public:
  float Utilization();  // TODO: See src/linux_processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif