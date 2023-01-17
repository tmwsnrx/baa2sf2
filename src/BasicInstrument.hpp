#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Instrument.hpp"
#include "types.hpp"

namespace z2sound
{

class BasicInstrument : public Instrument
{
public:
  struct KeyRegion
  {
    Key upper_key_limit;
    SampleId sample_id;
    Multiplier volume_multiplier;
    Multiplier pitch_multiplier;
  };

private:
  std::vector<size_t> oscillator_indices_;
  std::vector<KeyRegion> key_regions_;

  Multiplier volume_multiplier_;
  Multiplier pitch_multiplier_;
};

}
