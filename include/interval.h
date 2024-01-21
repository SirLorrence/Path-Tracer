#pragma once

#include "global.h"

struct Interval {
  // Default
  Interval() : m_min(+kInfinity), m_max(-kInfinity) {}
  Interval(double min, double max) : m_min(min), m_max(max) {}

  bool Contains(double value) { return m_min <= value && m_max >= value; }
  bool Surrounds(double value) { return m_min < value && m_max > value; }

  double Clamp(double value) const {
    if (value < m_min)
      return m_min;
    if (value > m_max)
      return m_max;
    return value;
  }

  static const Interval kEmpty;
  static const Interval kUniverse;

  double m_min;
  double m_max;
};

const static Interval kEmpty(+kInfinity, -kInfinity);
const static Interval kUniverse(-kInfinity, +kInfinity);
