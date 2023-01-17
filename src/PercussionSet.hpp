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
    float f1;
    float f2;

    Pan pan;
    uint16_t release;

    uint8_t b2;
    SampleId sample_id;

    float f3;
    float f4;
  };

  std::optional<const PercussionEntry&> get_entry(Key key) const;
  void set_entry(Key key, PercussionEntry entry);

private:
  std::map<Key, PercussionEntry> key_map_;
};

}
