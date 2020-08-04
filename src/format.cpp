#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int sec, min, hours = 0;

  hours = seconds / (3600);
  min = (seconds % 3600) / 60;
  sec = (seconds % 3600) % 60;

  std::stringstream stream;

  stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << min << ":" << std::setw(2) << std::setfill('0') << sec;

  return stream.str();
}