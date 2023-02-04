#pragma once

#include <cstdint>
#include <map>

#include "InstrumentBank.hpp"
#include "WaveBank.hpp"

namespace z2sound
{

class AudioArchive
{
friend class AudioArchiveLoader;

public:

private:
  std::map<uint8_t, WaveBank> wave_banks_;
  std::map<uint8_t, InstrumentBank> instrument_banks_;
};

}
