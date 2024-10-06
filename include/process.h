#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // DONE: See src/process.cpp
  int Uid();                               // DONE: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  Process(int pId, int uId, string user, string command, float cpuUtil,
          string ram, long int upTime)
      : _pId(pId),
        _uId(uId),
        _user(user),
        _command(command),
        _cpuUtilization(cpuUtil),
        _ram(ram),
        _upTime(upTime) {}

  // DONE: Declare any necessary private members
 private:
  int _pId;
  int _uId;
  std::string _user;
  std::string _command;
  float _cpuUtilization;
  std::string _ram;
  long int _upTime;
};

#endif