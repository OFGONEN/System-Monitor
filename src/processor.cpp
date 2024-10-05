#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// DONE: Return the aggregate CPU utilization
/*
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/
float Processor::Utilization() {
  vector<string> cpuUtilValues = LinuxParser::CpuUtilization();

  float totalTime = std::stol(cpuUtilValues[LinuxParser::kUser_]) +
                    std::stol(cpuUtilValues[LinuxParser::kNice_]) +
                    std::stol(cpuUtilValues[LinuxParser::kSystem_]) +
                    std::stol(cpuUtilValues[LinuxParser::kIdle_]) +
                    std::stol(cpuUtilValues[LinuxParser::kIOwait_]) +
                    std::stol(cpuUtilValues[LinuxParser::kIRQ_]) +
                    std::stol(cpuUtilValues[LinuxParser::kSoftIRQ_]) +
                    std::stol(cpuUtilValues[LinuxParser::kSteal_]);

  float idleTime = std::stol(cpuUtilValues[LinuxParser::kIdle_]) +
                   std::stol(cpuUtilValues[LinuxParser::kIOwait_]);

  float currentIdle = idleTime - prevIdle;
  float currentTotal = totalTime - prevTotal;

  float usage = (currentTotal - currentIdle) / currentTotal;

  prevIdle = idleTime;
  prevTotal = totalTime;

  return usage * 100;
}