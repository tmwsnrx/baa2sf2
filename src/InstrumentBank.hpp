#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Envelope.hpp"
#include "Instrument.hpp"
#include "Oscillator.hpp"

namespace z2sound
{

class InstrumentBank
{
    friend class BnkParser;

public:
    InstrumentBank(uint8_t id, uint8_t wave_bank_id);

    uint8_t
    get_id() const;

    uint8_t
    get_wave_bank_id() const;

    const std::vector<std::unique_ptr<Instrument>> &
    get_instruments() const;

    const std::vector<Oscillator> &
    get_oscillators() const;

private:
    uint8_t id_;
    uint8_t wave_bank_id_;
    std::vector<Envelope> envelopes_;
    std::vector<Oscillator> oscillators_;
    std::vector<std::unique_ptr<Instrument>> instruments_;
};

}
