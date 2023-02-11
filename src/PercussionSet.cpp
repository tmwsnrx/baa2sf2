#include "PercussionSet.hpp"

static constexpr float to_float_pan(uint8_t pan);

namespace z2sound
{

void PercussionSet::set_entry(Key key, PercussionSet::PercussionEntry entry)
{
  key_map_.insert_or_assign(key, entry);
}

Instrument::Type PercussionSet::get_type() const
{
  return Instrument::Type::Percussion;
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
      .pitch_multiplier = percussion_entry.pitch_multiplier * percussion_entry.pitch_multiplier_2,
      .pan = to_float_pan(percussion_entry.pan)
    };

    key_zones.emplace_back(std::move(key_zone));
  }

  return key_zones;
}

}

static constexpr float to_float_pan(uint8_t pan)
{
  if (pan == 64)
  {
    return 0.0f;
  }
  else if (pan < 64)
  {
    return (static_cast<float>(pan) / 63.0f) - 1.0f;
  }
  else
  {
    return static_cast<float>(pan - 64) / 63.0f;
  }
}