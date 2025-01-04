// Adapted from https://github.com/hcs64/vgm_ripping/blob/master/soundbank/wsyster/wsyster.c

#include "AfcDecoder.hpp"

static constexpr std::array<std::array<int16_t, 2>, 16> coefficients =
    {{
         {0x0000, 0x0000},
         {0x0800, 0x0000},
         {0x0000, 0x0800},
         {0x0400, 0x0400},
         {0x1000, -0x0800},
         {0x0e00, -0x0600},
         {0x0c00, -0x0400},
         {0x1200, -0x0a00},
         {0x1068, -0x08c8},
         {0x12c0, -0x08fc},
         {0x1400, -0x0c00},
         {0x0800, -0x0800},
         {0x0400, -0x0400},
         {-0x0400, 0x0400},
         {-0x0400, 0x0000},
         {-0x0800, 0x0000}
     }};

namespace z2sound
{

void
AfcDecoder::decode_buffer(std::span<const uint8_t, 9> encoded_samples, std::span<int16_t, 16> destination_buffer)
{
    auto source = encoded_samples.begin();
    auto destination = destination_buffer.begin();

    auto delta = static_cast<int16_t>(1 << (((*source) >> 4) & 0xf));
    auto index = static_cast<int16_t>((*source) & 0xf);

    source++;

    std::array<int8_t, 16> nibbles{};
    for (auto nibble = nibbles.begin(); nibble != nibbles.end();)
    {
        *nibble++ = static_cast<int8_t>(*source >> 4);
        *nibble++ = static_cast<int8_t>(*source & 0xf);
        source++;
    }

    for (auto &nibble : nibbles)
    {
        if (nibble >= 8)
        {
            nibble -= 16;
        }
    }

    for (auto &nibble : nibbles)
    {
        int32_t sample = (delta * nibble) << 11;
        sample += (static_cast<int32_t>(hist1_) * coefficients[index][0]) +
            (static_cast<int32_t>(hist2_) * coefficients[index][1]);
        sample >>= 11;

        if (sample > 32767)
        {
            sample = 32767;
        }
        else if (sample < -32768)
        {
            sample = -32768;
        }

        *destination++ = static_cast<int16_t>(sample);

        hist2_ = hist1_;
        hist1_ = static_cast<int16_t>(sample);
    }
}

}
