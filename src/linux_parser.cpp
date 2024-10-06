#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // MemTotal:       131919132 kB
  // MemFree:        19930064 kB
  float total = -100;
  float free = -100;

  std::string line;
  std::string key;
  std::string value;

  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
  if (fileStream.is_open()) {
    // Loop until both Total and Free is assigned.
    while ((total < 0 || free < 0) && std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key;

      if (key == "MemTotal:") {
        lineStream >> value;
        total = std::stof(value);
        // std::cout << "Total: " << total << std::endl;
      } else if (key == "MemFree:") {
        lineStream >> value;
        free = std::stof(value);
        // std::cout << "Free: " << free << std::endl;
      }
    }
  }

  // Utiliazed Memory = (Total - Free) / Total;
  return (total - free) / total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  // 31620.67 757943.01
  // This file contains two numbers (values in seconds): the uptime of the
  // system (including time spent in suspend) and the amount of time spent in
  // the idle process. We will need the first value for the Up Time

  std::string line;
  std::string time;
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> time;
    return std::stol(time);
  } else
    return -1;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  // /proc/stat
  // cpu  58108 220 48109 88871914 4450 0 3463 0 0 0
  // The First Line of the File is the Line we need
  vector<std::string> cpuUtilValues;
  string line;
  string key;
  string kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, kSteal, kGuest,
      kGuestNice;

  std::ifstream fileStream(kProcDirectory + kStatFilename);
  std::getline(fileStream, line);
  std::istringstream lineStream(line);

  lineStream >> key >> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> kIRQ >>
      kSoftIRQ >> kSteal >> kGuest >> kGuestNice;

  cpuUtilValues.push_back(kUser);
  cpuUtilValues.push_back(kNice);
  cpuUtilValues.push_back(kSystem);
  cpuUtilValues.push_back(kIdle);
  cpuUtilValues.push_back(kIOwait);
  cpuUtilValues.push_back(kIRQ);
  cpuUtilValues.push_back(kSoftIRQ);
  cpuUtilValues.push_back(kSteal);
  cpuUtilValues.push_back(kGuest);
  cpuUtilValues.push_back(kGuestNice);

  return cpuUtilValues;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // /proc/stat file
  // processes 175746
  std::string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> line;

      if (line == "processes") {
        std::string value;
        lineStream >> value;
        return std::stol(value);
      }
    }
  }
  return -1;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  // /proc/stat file
  // procs_running 2
  std::string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> line;

      if (line == "procs_running") {
        std::string value;
        lineStream >> value;
        return std::stol(value);
      }
    }
  }
  return -1;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  // proc/[pid]/cmdline
  string command = string();
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, command);
  }

  return command;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  // /proc/pid/status
  // VmSize:   165884 kB

  string line;
  string key;
  string value;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key;
      if (key == "VmSize:") {
        lineStream >> value;
        return std::to_string(std::stol(value) / 1000);
      }
    }
  }

  return string();
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  // /proc/[PID]/status
  // Uid:    0       0       0       0
  string line;
  string key;
  string value = "-1";

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      if (lineStream >> key && key == "Uid:") {
        lineStream >> value;
        return value;
      }
    }
  }

  return value;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // etc/passwd
  // tcpdump:x:107:113::/nonexistent:/usr/sbin/nologin
  // 107 is the uID that we need to match
  string userName = string();
  string uId = Uid(pid);

  string line;
  string other;
  string key;

  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> userName;
      lineStream >> other >> key;

      if (key == uId) return userName;
    }
  }

  return userName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  // /proc/1/stat
  // 1 (systemd) S 0 1 1 0 -1 4194560 9258 192992 109 2394 7233 486 576 209 20 0
  // 1 0 334 169865216 2813 18446744073709551615 1 1 0 0 0 0 671173123 4096 1260
  // 0 0 0 17 2 0 0 0 0 0 0 0 0 0 0 0 0 0
  // 22nd value is the value we want which is the starttime of the process
  // uptime = totaltime - starttime

  string line;
  string value;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);

    std::istringstream lineStream(line);

    for (int i = 1; i <= 22; i++) lineStream >> value;

    long totalUp = UpTime();
    long seconds = std::stol(value) / sysconf(_SC_CLK_TCK);

    return totalUp - seconds;
  }

  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  // /proc/pid/stat
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // #14 utime, #15 stime, #16 cutime, #17 cstime, #22 starttime

  long utime = 0;      // #14
  long stime = 0;      // #15
  long startTime = 0;  // #22 starttime
  long totalUptime = UpTime();
  float usage = 0;

  string line;
  string value;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);

    for (int i = 1; i <= 22; i++) {
      lineStream >> value;
      if (i == 14)
        utime = std::stol(value);
      else if (i == 15)
        stime = std::stol(value);
      else if (i == 22)
        startTime = std::stol(value);
    }

    long totalTimeSpend = utime + stime;

    long startTime_seconds = startTime / sysconf(_SC_CLK_TCK);
    long totalTimeSpend_seconds = totalTimeSpend / sysconf(_SC_CLK_TCK);

    long upTime = totalUptime - startTime_seconds;

    if (upTime != 0) usage = totalTimeSpend_seconds / upTime;
  }

  return usage;
}