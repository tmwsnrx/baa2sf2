#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace z2sound
{

class AfcDecoder
{
public:
    void
    decode_buffer(std::span<const uint8_t, 9> encoded_samples, std::span<int16_t, 16> destination_buffer);

private:
    int16_t hist_;
    int16_t hist2_;
};

}
