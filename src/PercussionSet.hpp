#pragma once

#include <map>
#include <optional>

#include "Instrument.hpp"
#include "types.hpp"

namespace z2sound
{

class PercussionSet : public Instrument
{
public:
  struct PercussionEntry
  {
    float volume_multiplier_2; // as part of the only velocity region
    float pitch_multiplier_2; // as part of the only velocity region

    Pan pan;
    uint16_t release;

    SampleId sample_id;

    float volume_multiplier;
    float pitch_multiplier;
  };

  void set_entry(Key key, PercussionEntry entry);

  virtual Instrument::Type get_type() const override;
  virtual std::vector<Instrument::KeyZone> get_key_zones() const override;

private:
  std::map<Key, PercussionEntry> key_map_;
};

}
