#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

System::System() { UpdateProcesses(); }

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // UpdateProcesses();
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's kernel identifier (string)
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::UpdateProcesses() {
  // convert pids into Proceesses
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();
  for (const int& i : pids) {
    Process process =
        Process(i, std::stoi(LinuxParser::Uid(i)), LinuxParser::User(i),
                LinuxParser::Command(i), LinuxParser::CpuUtilization(i),
                LinuxParser::Ram(i), LinuxParser::UpTime(i));

    processes_.push_back(process);
  }
}