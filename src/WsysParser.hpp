#pragma once

#include <cstdint>
#include <istream>
#include <optional>
#include <string>
#include <vector>

#include "Poco/BinaryReader.h"
#include "Poco/Logger.h"

#include "WaveBank.hpp"

namespace z2sound
{

class WsysParser
{
public:
  WsysParser(uint32_t group, std::istream& stream, std::streamoff base_offset, Poco::Logger& logger);

  std::optional<WaveBank> parse();

private:
  uint32_t group_;
  std::streamoff base_offset_;

  Poco::BinaryReader reader_;
  Poco::Logger& logger_;
};
  
}