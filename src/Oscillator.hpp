#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "Envelope.hpp"

namespace z2sound
{

struct Oscillator
{
    uint8_t b1;

    float value_offset;
    float value_multiplier;
    float time_scale;

    std::optional<std::vector<Envelope>::const_iterator> pre_sustain = std::nullopt;
    std::optional<std::vector<Envelope>::const_iterator> post_sustain = std::nullopt;
};

}