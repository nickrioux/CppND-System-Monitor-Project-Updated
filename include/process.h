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
  virtual std::string User();            // TODO: See src/process.cpp
  virtual std::string Command();         // TODO: See src/process.cpp
  virtual float CpuUtilization() const;  // TODO: See src/process.cpp
  virtual void ComputeCpuUtilization(long, long);
  virtual std::string Ram();  // TODO: See src/process.cpp
  virtual long int UpTime();  // TODO: See src/process.cpp
  void SetActive(bool);
  bool Active() const;
  void SetPrevTotalTime(long time) { prev_TotalTime = time; }
  long GetPrevTotalTime() const { return (prev_TotalTime); }
  void SetPrevProcessTime(long time) { prev_ProcessTime = time; }
  long GetPrevProcessTime() const { return (prev_ProcessTime); }
  void SetCpuUsage(float cpuUsage) { cpuUsage_ = cpuUsage; }
  bool operator<(Process const a) const;  // TODO: See src/process.cpp
  bool operator>(Process const a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  bool active_{true};
  pid_t pid_;
  std::string user_;
  std::string command_;
  float cpuUsage_;
  long prev_TotalTime{0};
  long prev_ProcessTime{0};
};

#endif
