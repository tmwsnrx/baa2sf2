#pragma once

#include <memory>
#include <vector>

#include "Instrument.hpp"

namespace z2sound {

class InstrumentBank
{
private:
  uint8_t id;
  std::vector<std::unique_ptr<Instrument>> instruments_;
};

}
