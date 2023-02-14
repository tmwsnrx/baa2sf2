#pragma once

#include <cstdint>

namespace z2sound
{

struct Envelope
{
  enum class CurveType
  {
    Linear = 0,
    Square = 1,
    SquaeRoot = 2,
    SampleCell = 3,

    // Special commands
    Jump = 0xd,
    Sustain = 0xe,
    Stop = 0xf
  };

  uint16_t curve_type;
  uint16_t time;
  
  union
  {
    uint16_t volume;
    uint16_t index;
  };

  CurveType get_curve_type() const
  {
    return static_cast<CurveType>(curve_type);
  }
};

}