#pragma once

#include <istream>
#include <optional>

#include "Poco/BinaryReader.h"
#include "Poco/Logger.h"

#include "InstrumentBank.hpp"

namespace z2sound
{

class BnkParser
{
public:
  BnkParser(uint32_t group, std::istream& stream, Poco::Logger& logger);
  std::optional<InstrumentBank> parse();

private:
  uint32_t group_;
  Poco::BinaryReader reader_;
  Poco::Logger& logger_;
};

}
