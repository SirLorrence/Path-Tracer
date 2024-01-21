#pragma once

#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <string>
#include <limits>
#include <memory>
#include <thread>
#include <random>

constexpr double kInfinity = std::numeric_limits<double>::infinity();
constexpr double kPi = 3.1415926535897932385;
const uint32_t g_ThreadsAvailable = static_cast<uint32_t>(std::thread::hardware_concurrency());

inline double DegreesToRadians(double degrees) { return degrees * kPi / 180.0; }

// returns a value between 0 - 1
inline double RandomDouble01() {
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  thread_local std::mt19937 generator;
  return distribution(generator);
}

inline double RandomDouble(double min, double max) {
  return min + (max - min) * RandomDouble01();
}

static std::string GetTimeStamp() {
  // Get the current time
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  // Create the time stamp
  std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);

  // Convert timestamp to tm
  std::tm* timeInfo = std::localtime(&timeStamp);

  // Formatting
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

  return static_cast<std::string>(buffer);
}


// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "util.h"
