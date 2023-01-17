#pragma once

#include <cstdint>
#include <map>

#include "InstrumentBank.hpp"

namespace z2sound
{

class AudioArchive
{
public:

private:
  std::map<uint8_t, InstrumentBank> instrument_banks_;
};

}
