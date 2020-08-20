#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  virtual float Utilization() = 0;

 protected:
  float computeCpuUtilization(long activeJiffies, long idleJiffies);
  long getPrevActiveJiffies() const { return prevActiveJiffies_; }
  long getPrevIdleJiffies() const { return prevIdleJiffies_; }
  void setPrevActiveJiffies(const long activeJiffies) {
    prevActiveJiffies_ = activeJiffies;
  }
  void setPrevIdleJiffies(const long idleJiffies) {
    prevIdleJiffies_ = idleJiffies;
  }

 private:
  long prevActiveJiffies_{0};
  long prevIdleJiffies_{0};
};

#endif