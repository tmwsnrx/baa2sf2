#pragma once

#include <cstdint>

namespace z2sound
{

struct Envelope
{
  uint16_t curve_type;
  uint16_t time;
  
  union
  {
    uint16_t volume;
    uint16_t index;
  };
};

}