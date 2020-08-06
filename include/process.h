#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(const int& pid, string user, string command);
  virtual int Pid() const;               // TODO: See src/process.cpp
  virtual std::string User();            // TODO: See src/process.cpp
  virtual std::string Command();         // TODO: See src/process.cpp
  virtual float CpuUtilization() const;  // TODO: See src/process.cpp
  virtual void ComputeCpuUtilization(long, long);
  virtual std::string Ram();  // TODO: See src/process.cpp
  virtual long int UpTime();  // TODO: See src/process.cpp
  void SetActive(bool);
  bool Active() const;
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