#include "format.h"

#include <string>

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long seconds) {
  std::string elapsedTime = std::string();

  long hour = seconds / 3600;
  long minute = (seconds / 60) % 60;
  long second = seconds % 60;

  elapsedTime += hour < 10 ? "0" + std::to_string(hour) : std::to_string(hour);
  elapsedTime += ":";
  elapsedTime +=
      minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
  elapsedTime += ":";
  elapsedTime +=
      second < 10 ? "0" + std::to_string(second) : std::to_string(second);

  return elapsedTime;
}