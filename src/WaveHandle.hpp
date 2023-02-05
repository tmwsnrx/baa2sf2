#pragma once

#include <optional>
#include <span>

#include "WaveInfo.hpp"

namespace z2sound
{

using WaveBuffer = std::span<const int16_t>;

class WaveHandle
{
public:
  explicit WaveHandle(WaveInfo wave_info);

  const WaveInfo& get_wave_info() const;
  std::optional<WaveBuffer> get_buffer() const;

  void set_buffer(WaveBuffer wave_buffer);

private:
  WaveInfo wave_info_;
  std::optional<WaveBuffer> buffer_;
};

}
