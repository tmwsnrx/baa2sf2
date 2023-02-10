#pragma once

#include <vector>

#include "types.hpp"

namespace z2sound
{

class Instrument
{
public:
  struct KeyZone
  {
    Key lower_key_limit;
    Key upper_key_limit;
    uint16_t wave_id;
    float volume_multiplier;
    float pitch_multiplier;
  };

  virtual ~Instrument() = default;

  virtual std::vector<KeyZone> get_key_zones() const = 0;
};

}
