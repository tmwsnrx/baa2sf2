#pragma once

#include <cstdint>

namespace z2sound
{

struct WaveInfo
{
  uint8_t codec;
  uint8_t root_key;

  float sample_rate;
  uint32_t data_offset;
  uint32_t data_length;

  int32_t loop_indicator;
  uint32_t loop_start;
  uint32_t loop_end;

  uint32_t num_samples;

  uint16_t _unknown3;
  uint16_t _unknown4;
};

}
