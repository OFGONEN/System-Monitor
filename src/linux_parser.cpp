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
  std::ifstream fileStream(kProcDirectory + "/" + std::to_string(pid) +
                           kCmdlineFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, command);
  }

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  // /proc/[PID]/status
  // Uid:    0       0       0       0
  string line;
  string key;
  string value = "-1";

  std::ifstream fileStream(kProcDirectory + "/" + std::to_string(pid) +
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

  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // etc/passwd
  //tcpdump:x:107:113::/nonexistent:/usr/sbin/nologin
  //107 is the uID that we need to match
  string userName = string();

  return userName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

float LinuxParser::CpuUtilization(int pid) { return 0; }