#pragma once

#include <vector>

#include "Instrument.hpp"

namespace z2sound {

class InstrumentBank
{
private:
  uint8_t id;
  std::vector<Instrument> instruments_;
};

}
