#ifndef MACOSX_SYSTEM_INFO_H
#define MACOSX_SYSTEM_INFO_H

#include <mach/mach_host.h>
#include <sys/sysctl.h>
#include <sys/types.h>

#include <fstream>
#include <regex>
#include <set>
#include <string>

class ProcessMacOSX {
 public:
  ProcessMacOSX(long pid) : pid_(pid){};
  ProcessMacOSX(struct kinfo_proc procInfo, long ram, long activeJiffies)
      : pid_(procInfo.kp_proc.p_pid),
        ram_(ram),
        activeJiffies_(activeJiffies),
        procInfo_(procInfo){};
  struct kinfo_proc &GetProcInfo() const {
    return (const_cast<struct kinfo_proc &>(procInfo_));
  }
  long GetRam() const { return (ram_); }
  long GetActiveJiffies() const { return (activeJiffies_); }
  bool operator<(ProcessMacOSX const a) const { return (pid_ < a.pid_); }

 private:
  long pid_;
  long ram_;
  long activeJiffies_;
  struct kinfo_proc procInfo_;
};

// Implemented as a Singleton
class MacOSXInfo {
 public:
  // Instance
  static MacOSXInfo *Instance() {
    if (instance_ == 0) {
      instance_ = new MacOSXInfo;
      instance_->init();
    }

    return (instance_);
  }

  // System
  float MemoryUtilization();
  long UpTime();
  std::set<int> Pids();
  int TotalProcesses();
  int RunningProcesses();
  std::string OperatingSystem();
  std::string Kernel();
  std::vector<long> CpuUtilization();
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();

  // Processes
  void UpdateProcesses();
  std::string Command(int pid);
  std::string Ram(int pid);
  std::string Uid(int pid);
  std::string User(int pid);
  long int UpTime(int pid);
  std::vector<std::string> CpuUtilization(int pid);
  std::vector<long> CpuTimes(int pid);

 private:
  static MacOSXInfo *instance_;

  // Private methods
  void init();
  int getCpuInfo(unsigned long *pulSystem, unsigned long *pulUser,
                 unsigned long *pulNice, unsigned long *pulIdle);
  int getProcessList(kinfo_proc **procList, size_t *procCount);
  std::string getCmdLine(struct kinfo_proc *k, int &basenameOffset);

  // CPU
  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };

  // CPU Info
  int cpuCount_{0};
  std::vector<long> cpuStats_{};

  // Process Info
  long iRunningProcess_{0};
  std::set<ProcessMacOSX> processSet_{};
};

#endif
