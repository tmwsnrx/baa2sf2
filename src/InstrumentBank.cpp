#include "InstrumentBank.hpp"

namespace z2sound
{

InstrumentBank::InstrumentBank(uint8_t id, uint8_t wave_bank_id)
: id_{id}, wave_bank_id_{wave_bank_id} {}

uint8_t InstrumentBank::get_id() const
{
  return id_;
}

uint8_t InstrumentBank::get_wave_bank_id() const
{
  return wave_bank_id_;
}

}