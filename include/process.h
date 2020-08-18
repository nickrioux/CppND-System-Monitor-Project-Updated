#ifndef PROCESS_H
#define PROCESS_H

#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(const int& pid, string user, string command);
  virtual ~Process(){};
  virtual int Pid() const;               // TODO: See src/process.cpp
  virtual std::string User() const;            // TODO: See src/process.cpp
  virtual std::string Command() const;         // TODO: See src/process.cpp
  virtual float CpuUtilization() const;  // TODO: See src/process.cpp
  virtual void ComputeCpuUtilization(long, long);
  virtual std::string Ram() const;  //Specific for each version
  virtual long int UpTime() const;  //Specific for each version
  void SetActive(bool);
  bool Active() const;
  void SetCpuUsage(float cpuUsage) { cpuUsage_ = cpuUsage; }
  bool operator<(Process const a) const;  // TODO: See src/process.cpp
  bool operator>(Process const a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  void setPrevTotalTime(long time) { prev_TotalTime_ = time; }
  long getPrevTotalTime() const { return (prev_TotalTime_); }
  void setPrevProcessTime(long time) { prev_ProcessTime_ = time; }
  long getPrevProcessTime() const { return (prev_ProcessTime_); }

  bool active_{true};
  pid_t pid_;
  std::string user_;
  std::string command_;
  float cpuUsage_;
  long prev_TotalTime_{0};
  long prev_ProcessTime_{0};
};

#endif
