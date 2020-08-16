
#include <dirent.h>
#include <mach/mach.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/proc.h>
#include <pwd.h>
#include <unistd.h>
#include <libproc.h>
#include <mach/mach_host.h>


#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "macosx_info.h"

using std::stof;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

MacOSXInfo * MacOSXInfo::instance_ = 0;


// DONE: An example of how to read data from the filesystem
string MacOSXInfo::OperatingSystem() {
  int mib[2];
  size_t len;
  char* osName;

  mib[0] = CTL_KERN;
  mib[1] = KERN_OSTYPE;
  sysctl(mib, 2, NULL, &len, NULL, 0);
  osName = new char[len];
  sysctl(mib, 2, osName, &len, NULL, 0);

  string sValue(osName);

  delete[] osName;

  return sValue;
}

// DONE: An example of how to read data from the filesystem
string MacOSXInfo::Kernel() {
  int mib[2];
  size_t len;
  char* osKernel;

  mib[0] = CTL_KERN;
  mib[1] = KERN_VERSION;
  sysctl(mib, 2, NULL, &len, NULL, 0);
  osKernel = new char[len];
  sysctl(mib, 2, osKernel, &len, NULL, 0);

  string sKernel(osKernel);

  delete[] osKernel;

  return sKernel;
}

void MacOSXInfo::UpdateProcesses() {
    kinfo_proc *mylist =NULL;
    size_t mycount = 0;

    getProcessList(&mylist, &mycount);

    MacOSXInfo::processSet.clear();

    MacOSXInfo::iRunningProcess = 0;

    //CPU Information
    vector<long> cpu_stats;
    cpu_stats_ = CpuUtilization();

    for (int i = 0; i < static_cast<int>(mycount); i++) {
        struct kinfo_proc *currentProcess = &mylist[i];
        struct proc_taskinfo pti;
        long ram = 0;
        long activeJiffies = 0;

        if(sizeof(pti) == proc_pidinfo(currentProcess->kp_proc.p_pid, PROC_PIDTASKINFO, 0, &pti, sizeof(pti)))
        {
            MacOSXInfo::iRunningProcess += pti.pti_numrunning;
            ram = (pti.pti_resident_size) / (1024*1024);
            activeJiffies = (pti.pti_total_system + pti.pti_total_user);
        }

        MacOSXInfo::processSet.insert(ProcessMacOSX(*currentProcess,ram,activeJiffies));
    }

    delete[] mylist;
}

// BONUS: Update this to use std::filesystem
std::set<int> MacOSXInfo::Pids() {
    std::set<int> pids;

    for (const ProcessMacOSX & p : MacOSXInfo::processSet) {
        pids.insert(p.GetProcInfo().kp_proc.p_pid);
    }
    return pids;
}

// TODO: Read and return the system memory utilization
float MacOSXInfo::MemoryUtilization() {
    kern_return_t kr;
    vm_statistics64_data_t vmstat;
    mach_msg_type_number_t count2 = sizeof(vmstat) / sizeof(natural_t);

    kr = host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vmstat, &count2);
    if (kr != KERN_SUCCESS) {
        return kr;
    }

    long total = (vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count);

    long appMemory =  (vmstat.internal_page_count - vmstat.purgeable_count);
    long cacheMemory = (vmstat.external_page_count + vmstat.purgeable_count);

    float memoryUsage = (appMemory + cacheMemory)/static_cast<float>(total);

    return (memoryUsage);
}

// TODO: Read and return the system uptime
long MacOSXInfo::UpTime() {
  struct timeval bootTime, currTime;
  int mib[2] = {CTL_KERN, KERN_BOOTTIME};
  size_t size = sizeof(bootTime);

  int err = sysctl(mib, 2, &bootTime, &size, NULL, 0);
  if (err) {
    return -1;
  }
  gettimeofday(&currTime, NULL);

  return (long)difftime(currTime.tv_sec, bootTime.tv_sec);
}

// Read and return the number of jiffies for the system
long MacOSXInfo::Jiffies() {
    if (cpu_stats_.size() > 0) {
      return(cpu_stats_[kNice_]+cpu_stats_[kSystem_]+cpu_stats_[kUser_]+cpu_stats_[kIdle_]);
    }
    else 
    {
        return 0;
    }
}

// Read and return the number of active jiffies for a PID
long MacOSXInfo::ActiveJiffies(int pid) {
  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
      return((*p).GetActiveJiffies()*cpu_count_);
  }

  return(0);
}

// TODO: Read and return the number of active jiffies for the system
long MacOSXInfo::ActiveJiffies() {
  if (cpu_stats_.size() > 0) {
    return (cpu_stats_[kNice_]+cpu_stats_[kSystem_]+cpu_stats_[kUser_]);
  }
  else
  {
    return 0;
  }
}

// Read and return the number of idle jiffies for the system
long MacOSXInfo::IdleJiffies() {
  if (cpu_stats_.size() > 0) {
    return (cpu_stats_[MacOSXInfo::kIdle_]);
  } else {
    return (0);
  }
}

// TODO: Read and return CPU utilization
vector<long> MacOSXInfo::CpuUtilization() {
  vector<long> cpuJiffies;

  unsigned long ulSystem;
  unsigned long ulUser;
  unsigned long ulNice;
  unsigned long ulIdle;

  cpu_count_ = getCpuInfo(&ulSystem, &ulUser, &ulNice, &ulIdle);

  cpuJiffies.push_back(ulSystem);
  cpuJiffies.push_back(ulUser);
  cpuJiffies.push_back(ulNice);
  cpuJiffies.push_back(ulIdle);

  return (cpuJiffies);
}

// TODO: Read and return the total number of processes
int MacOSXInfo::TotalProcesses() {
    return (processSet.size());
}

// TODO: Read and return the number of running processes
int MacOSXInfo::RunningProcesses() {
  return (MacOSXInfo::iRunningProcess);
}

// TODO: Read and return the command associated with a process
string MacOSXInfo::Command(int pid) {

  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
      return(string(p->GetProcInfo().kp_proc.p_comm));
  }

  return("");
}

// TODO: Read and return the memory used by a process
string MacOSXInfo::Ram(int pid) {

  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
    return(to_string((*p).GetRam()));
  }

  return "";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string MacOSXInfo::Uid(int pid) {
  string uid{""};
  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
      uid = to_string((*p).GetProcInfo().kp_eproc.e_ucred.cr_uid);
  }

  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string MacOSXInfo::User(int pid) {
  string sUser{""};

  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
    struct passwd *user = getpwuid((*p).GetProcInfo().kp_eproc.e_ucred.cr_uid);

    if (user) {
        sUser = string(user->pw_name);
    }
  }

  return sUser;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long MacOSXInfo::UpTime(int pid) {
  long value = 0;

  auto p = processSet.find(ProcessMacOSX(pid));

  if (p != processSet.end())
  {
      value = p->GetActiveJiffies()/10000000/100;
  }

  return value;
}

//Private Methods
void MacOSXInfo::init() {
        cpu_stats_ = CpuUtilization();
}

int MacOSXInfo::getCpuInfo(unsigned long* pulSystem, unsigned long* pulUser,
                             unsigned long* pulNice, unsigned long* pulIdle) {
  mach_msg_type_number_t unCpuMsgCount = 0;
  processor_flavor_t nCpuFlavor = PROCESSOR_CPU_LOAD_INFO;
  
  natural_t unCPUNum = 0;
  processor_cpu_load_info_t structCpuData;
  host_t host = mach_host_self();
  *pulSystem = 0;
  *pulUser = 0;
  *pulNice = 0;
  *pulIdle = 0;
  host_processor_info(host, nCpuFlavor, &unCPUNum,
                             (processor_info_array_t*)&structCpuData,
                             &unCpuMsgCount);
  for (int i = 0; i < (int)unCPUNum; i++) {
    *pulSystem += structCpuData[i].cpu_ticks[CPU_STATE_SYSTEM];
    *pulUser += structCpuData[i].cpu_ticks[CPU_STATE_USER];
    *pulNice += structCpuData[i].cpu_ticks[CPU_STATE_NICE];
    *pulIdle += structCpuData[i].cpu_ticks[CPU_STATE_IDLE];
  }

  return unCPUNum;
}

//From Apple Dev Web Site
int MacOSXInfo::getProcessList(kinfo_proc **procList, size_t *procCount)
// Returns a list of all BSD processes on the system.  This routine
// allocates the list and puts it in *procList and a count of the
// number of entries in *procCount.  You are responsible for freeing
// this list (use "free" from System framework).
// On success, the function returns 0.
// On error, the function returns a BSD errno value.
{
    int                 err;
    kinfo_proc *        result;
    bool                done;
    static const int    name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    // Declaring name as const requires us to cast it when passing it to
    // sysctl because the prototype doesn't include the const modifier.
    size_t              length;

    *procCount = 0;

    // We start by calling sysctl with result == NULL and length == 0.
    // That will succeed, and set length to the appropriate length.
    // We then allocate a buffer of that size and call sysctl again
    // with that buffer.  If that succeeds, we're done.  If that fails
    // with ENOMEM, we have to throw away our buffer and loop.  Note
    // that the loop causes use to call sysctl with NULL again; this
    // is necessary because the ENOMEM failure case sets length to
    // the amount of data returned, not the amount of data that
    // could have been returned.

    result = NULL;
    done = false;
    do {
        // Call sysctl with a NULL buffer.

        length = 0;
        err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                     NULL, &length,
                     NULL, 0);
        if (err == -1) {
            err = errno;
        }

        // Allocate an appropriately sized buffer based on the results
        // from the previous call.

        if (err == 0) {
            result = (kinfo_proc * )new kinfo_proc[length];
            if (result == NULL) {
                err = ENOMEM;
            }
        }

        // Call sysctl again with the new buffer.  If we get an ENOMEM
        // error, toss away our buffer and start again.

        if (err == 0) {
            err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                         result, &length,
                         NULL, 0);
            if (err == -1) {
                err = errno;
            }
            if (err == 0) {
                done = true;
            } else if (err == ENOMEM) {
                delete[] result;
                result = NULL;
                err = 0;
            }
        }
    } while (err == 0 && ! done);

    // Clean up and establish post conditions.

    if (err != 0 && result != NULL) {
        delete[] result;
        result = NULL;
    }
    *procList = result;
    if (err == 0) {
        *procCount = length / sizeof(kinfo_proc);
    }

    return err;
}
