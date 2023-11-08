#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <string>
#include <limits>
#include <memory>
#include <vector>
#include <thread>

constexpr double kInfinity = std::numeric_limits<double>::infinity();
constexpr double kPi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) { return degrees * kPi / 180.0; }

// returns a value between 0 - 1
inline double RandomDouble01() { return rand() / (RAND_MAX + 1.0); }

inline double RandomDouble(double min, double max) {
  return min + (max - min) * RandomDouble01();
}

static std::string GetTimeStamp(){
  // Get the current time
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  // Create the time stamp
  std::time_t time_stamp = std::chrono::system_clock::to_time_t(now);
  
  // Convert timestamp to tm
  std::tm *time_info = std::localtime(&time_stamp);

  // Formatting
  char buffer[80];
  strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S", time_info);

  return static_cast<std::string>(buffer);
}



// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "util.h"

#endif