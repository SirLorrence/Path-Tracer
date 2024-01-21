#pragma once
#include <iostream>
#include "interval.h"

using Color = Vec3;

inline double LinearToGamma(double linearComponent) {
  return std::sqrt(linearComponent);
}

inline Color WriteColor(Color pixelColor, int pixelSampleSize) {
  std::uint8_t colorRange = std::numeric_limits<std::uint8_t>::max(); // max color range for TinyPng
  double r = pixelColor.x();
  double g = pixelColor.y();
  double b = pixelColor.z();

  // divbing the color by n samples
  double scale = 1.0 / pixelSampleSize;
  r *= scale;
  b *= scale;
  g *= scale;

  // Apply the linear to gamma transform
  r = LinearToGamma(r);
  g = LinearToGamma(g);
  b = LinearToGamma(b);

  static const Interval intensity(0.000, 0.999);

  return {
    colorRange * intensity.Clamp(r),
    colorRange * intensity.Clamp(g),
    colorRange * intensity.Clamp(b)
  };
}
