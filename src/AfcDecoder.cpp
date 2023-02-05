#include "AfcDecoder.hpp"

static constexpr std::array<std::array<int16_t, 2>, 16> coefficients =
{{
  {0x0000, 0x0000},
  {0x0800, 0x0000},
  {0x0000, 0x0800},
  {0x0400, 0x0400},
  {0x1000, -2048},
  {0x0e00, -1536},
  {0x0c00, -1024},
  {0x1200, -2560},
  {0x1068, -2264},
  {0x12c0, -2300},
  {0x1400, -3072},
  {0x0800, -2048},
  {0x0400, -1024},
  {-1024, 0x0400},
  {-1024, 0x0000},
  {-2048, 0x0000}
}};

namespace z2sound
{

void AfcDecoder::decode_buffer(std::span<const uint8_t, 9> encoded_samples, std::span<int16_t, 16> destination_buffer)
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
    int32_t sample = (delta * nibble) << 11;
		sample += (hist_ * coefficients[index][0]) + (hist2_ * coefficients[index][1]);
		sample >>= 11;

		if(sample > 32767) {
			sample = 32767;
		}
		else if(sample < -32768) {
			sample = -32768;
		}

		*destination++ = static_cast<int16_t>(sample);

		hist2_ = hist_;
		hist_ = sample;
  }
}

}
