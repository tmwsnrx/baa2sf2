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
friend class BnkParser;

public:
  virtual Instrument::Type get_type() const override;
  virtual std::vector<Instrument::KeyZone> get_key_zones() const override;

private:
  struct KeyRegion
  {
    Key upper_key_limit;
    SampleId sample_id;
    Multiplier volume_multiplier;
    Multiplier pitch_multiplier;
  };

  std::vector<size_t> oscillator_indices_;
  std::vector<KeyRegion> key_regions_;

  Multiplier volume_multiplier_;
  Multiplier pitch_multiplier_;
};

}
