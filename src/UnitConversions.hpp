#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

struct Tuning
{
    int16_t coarse;
    int16_t fine;
};

static constexpr int16_t
adsr_ticks_to_timescents(uint16_t ticks)
{
    constexpr float ticks_per_second = 600.0f;

    if (ticks==0)
    {
        return std::numeric_limits<int16_t>::min();
    }

    return static_cast<int16_t>(std::round(1200.0f*std::log2(static_cast<float>(ticks)/ticks_per_second)));
}

constexpr int16_t
volume_to_attenuation(float volume_multiplier)
{
    if (volume_multiplier <= 0.0f)
    {
        return 1440;
    }
    else if (volume_multiplier >= 1.0f)
    {
        return 0;
    }

    return static_cast<int16_t>(std::round(1000.0f*std::log10(volume_multiplier)));
}

constexpr int16_t
volume_to_attenuation(uint16_t volume)
{
    float volume_multiplier = static_cast<float>(volume)/static_cast<float>(std::numeric_limits<int16_t>::max());
    return volume_to_attenuation(volume_multiplier);
}

constexpr Tuning
pitch_to_tuning(float pitch_multiplier)
{
    if (pitch_multiplier==1.0f)
    {
        return Tuning{
            .coarse = 0,
            .fine = 0};
    }

    auto cents_total = static_cast<int32_t>(std::round(1200.0f*std::log2(pitch_multiplier)));
    auto semitones = static_cast<int16_t>(cents_total/100);
    auto fine = static_cast<int16_t>(cents_total - semitones*100);

    return Tuning{
        .coarse = semitones,
        .fine = fine};
}

constexpr int16_t
pan_float_to_promille(float pan)
{
    if (pan <= -1.0f)
    {
        return -500;
    }
    else if (pan >= 1.0f)
    {
        return 500;
    }

    return static_cast<int16_t>(std::round(pan*500.0f));
}
