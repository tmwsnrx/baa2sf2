#include "BasicInstrument.hpp"

namespace z2sound
{

Instrument::Type
BasicInstrument::get_type() const
{
    return Instrument::Type::BasicInstrument;
}

std::vector<Instrument::KeyZone>
BasicInstrument::get_key_zones() const
{
    std::vector<Instrument::KeyZone> key_zones;

    for (Key lower_key = 0; auto key_region : key_regions_)
    {
        KeyZone key_zone{
            .lower_key_limit = lower_key,
            .upper_key_limit = key_region.upper_key_limit,
            .wave_id = key_region.sample_id,
            .volume_multiplier = key_region.volume_multiplier * volume_multiplier_,
            .pitch_multiplier = key_region.pitch_multiplier * pitch_multiplier_,
            .pan = 0.0f,

            .oscillator = oscillator_,
            .release = 0
        };

        key_zones.emplace_back(key_zone);
        lower_key = key_region.upper_key_limit + 1;
    }

    return key_zones;
}

}