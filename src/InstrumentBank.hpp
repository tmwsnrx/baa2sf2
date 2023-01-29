#pragma once

#include <memory>
#include <vector>

#include "Instrument.hpp"

namespace z2sound {

class InstrumentBank
{
friend class BnkParser;

public:
  struct Oscillator
  {
    uint8_t b1;

    float f1;
    float f2;

    uint32_t u1;

    uint32_t offset1;
    uint32_t offset2;
  };

private:
  uint8_t id;
  std::vector<std::unique_ptr<Instrument>> instruments_;
};

}
