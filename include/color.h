#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "interval.h"

using Color = Vec3;
inline double LinearToGamma(double linear_component) {
  return std::sqrt(linear_component);
}
inline Color WriteColor(Color pixel_color, int pixel_sample_size) {
  std::uint8_t color_range = std::numeric_limits<std::uint8_t>::max(); // max color range for TinyPng
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();

  // divbing the color by n samples
  double scale = 1.0 / pixel_sample_size;
  r *= scale;
  b *= scale;
  g *= scale;

  // Apply the linear to gamma transform
  r = LinearToGamma(r);
  g = LinearToGamma(g);
  b = LinearToGamma(b);

  static const Interval color_intensity(0.000, 0.999);

  return {color_range * color_intensity.Clamp(r),
          color_range * color_intensity.Clamp(g),
          color_range * color_intensity.Clamp(b)};
}

#endif