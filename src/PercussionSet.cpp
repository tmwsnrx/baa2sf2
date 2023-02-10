#include "PercussionSet.hpp"

namespace z2sound
{

void PercussionSet::set_entry(Key key, PercussionSet::PercussionEntry entry)
{
  key_map_.insert_or_assign(key, entry);
}

std::vector<Instrument::KeyZone> PercussionSet::get_key_zones() const
{
  std::vector<Instrument::KeyZone> key_zones;

  for (const auto& [key, percussion_entry] : key_map_)
  {
    KeyZone key_zone {
      .lower_key_limit = key,
      .upper_key_limit = key,
      .wave_id = percussion_entry.sample_id,
      .volume_multiplier = percussion_entry.volume_multiplier * percussion_entry.volume_multiplier_2,
      .pitch_multiplier = percussion_entry.pitch_multiplier * percussion_entry.pitch_multiplier_2
    };

    key_zones.emplace_back(std::move(key_zone));
  }

  return key_zones;
}

}