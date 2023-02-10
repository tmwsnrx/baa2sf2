#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Instrument.hpp"

namespace z2sound {

class InstrumentBank
{
friend class BnkParser;

public:
  struct Envelope
  {
    uint16_t curve_type;
    uint16_t time;
    
    union
    {
      uint16_t volume;
      uint16_t index;
    };
  };

  struct OscillatorConfig
  {
    uint8_t b1;

    float value_offset;
    float value_multiplier;
    float time_scale;

    std::optional<std::vector<Envelope>::const_iterator> pre_sustain = std::nullopt;
    std::optional<std::vector<Envelope>::const_iterator> post_sustain = std::nullopt;
  };

  InstrumentBank(uint8_t id, uint8_t wave_bank_id);

  uint8_t get_id() const;
  uint8_t get_wave_bank_id() const;

  const std::vector<std::unique_ptr<Instrument>>& get_instruments() const;

private:
  uint8_t id_;
  uint8_t wave_bank_id_;
  std::vector<Envelope> envelopes_;
  std::vector<OscillatorConfig> oscillators_;
  std::vector<std::unique_ptr<Instrument>> instruments_;
};

}
