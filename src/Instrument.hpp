#pragma once

#include <vector>

#include "types.hpp"

namespace z2sound
{

class Instrument
{
public:
  enum class Type
  {
    Percussion,
    BasicInstrument
  };

  struct KeyZone
  {
    Key lower_key_limit;
    Key upper_key_limit;
    uint16_t wave_id;
    float volume_multiplier;
    float pitch_multiplier;
    float pan;
  };

  virtual ~Instrument() = default;

  virtual Type get_type() const = 0;
  virtual std::vector<KeyZone> get_key_zones() const = 0;
};

}
