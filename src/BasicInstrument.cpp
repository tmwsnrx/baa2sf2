#include "BasicInstrument.hpp"

namespace z2sound
{

std::vector<Instrument::KeyZone> BasicInstrument::get_key_zones() const
{
  std::vector<Instrument::KeyZone> key_zones;

  for (Key lower_key = 0; auto key_region : key_regions_)
  {
    KeyZone key_zone {
      .lower_key_limit = lower_key,
      .upper_key_limit = key_region.upper_key_limit,
      .wave_id = key_region.sample_id,
      .volume_multiplier = key_region.volume_multiplier * volume_multiplier_,
      .pitch_multiplier = key_region.pitch_multiplier * pitch_multiplier_
    };

    key_zones.emplace_back(std::move(key_zone));
  }

  return key_zones;
}

}