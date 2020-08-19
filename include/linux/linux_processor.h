#include "processor.h"

#ifndef LINUX_PROCESSOR_H
#define LINUX_PROCESSOR_H

class Linux_Processor : public Processor {
 public:
  float Utilization();

 private:
};

#endif