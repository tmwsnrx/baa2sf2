#include "Adsr.hpp"

#include <cmath>
#include <limits>

static constexpr int16_t milliseconds_to_timescents(uint16_t milliseconds);

Adsr Adsr::from_oscillator(const z2sound::Oscillator& oscillator, const z2sound::InstrumentBank& bank)
{
  Adsr adsr{
    .attack_time = std::numeric_limits<int16_t>::min(),
    .decay_time = std::numeric_limits<int16_t>::min(),
    .sustain_attenuation = 0,
    .release_time = std::numeric_limits<int16_t>::min()
  };

  return adsr;
}

Adsr Adsr::from_percussion_release(uint16_t release)
{
  int16_t release_time = milliseconds_to_timescents(release & 0x3fff);

  return Adsr{
    .attack_time = std::numeric_limits<int16_t>::min(),
    .decay_time = std::numeric_limits<int16_t>::min(),
    .sustain_attenuation = 0,
    .release_time = release_time
  };
}

static constexpr int16_t milliseconds_to_timescents(uint16_t milliseconds)
{
  if (milliseconds == 0)
  {
    return std::numeric_limits<int16_t>::min();
  }

  return static_cast<int16_t>(std::round(1200.0f * std::log2(static_cast<float>(milliseconds) / 1000.0f)));
}