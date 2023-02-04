#include "AfcDecoder.hpp"

static constexpr std::array<std::array<int16_t, 2>, 16> coefficients =
{{
  {0x0000, 0x0000},
  {0x0800, 0x0000},
  {0x0000, 0x0800},
  {0x0400, 0x0400},
  {0x1000, 0xf800},
  {0x0e00, 0xfa00},
  {0x0c00, 0xfc00},
  {0x1200, 0xf600},
  {0x1068, 0xf738},
  {0x12c0, 0xf704},
  {0x1400, 0xf400},
  {0x0800, 0xf800},
  {0x0400, 0xfc00},
  {0xfc00, 0x0400},
  {0xfc00, 0x0000},
  {0xf800, 0x0000}
}};

namespace z2sound
{

AfcDecoder::AfcDecoder()
: hist_{}, hist2_{} {}



void AfcDecoder::decode_buffer(std::span<const uint8_t, 8> encoded_samples, std::span<int16_t, 16> destination_buffer)
{
  auto source = encoded_samples.begin();
  auto destination = destination_buffer.begin();

  int16_t delta = 1 << (((*source) >> 4) & 0xf);
  int16_t index = (*source) & 0xf;

	source++;

  std::array<int8_t, 16> nibbles;
  for (auto nibble = nibbles.begin(); nibble != nibbles.end();)
  {
    *nibble++ = *source >> 4;
    *nibble++ = *source & 0xf;
    source++;
  }

  for (auto& nibble : nibbles)
  {
    if (nibble >= 8)
    {
      nibble -= 16;
    }
  }

  for (auto& nibble : nibbles)
  {
    int16_t sample = (delta * nibble) << 11;
		sample += (hist_ * coefficients[index][0]) + (hist2_ * coefficients[index][1]);
		sample >>= 11;

		if(sample > 32767) {
			sample = 32767;
		}
		else if(sample < -32768) {
			sample = -32768;
		}

		*destination++ = sample;

		hist2_ = hist_;
		hist_ = sample;
  }
}

}
