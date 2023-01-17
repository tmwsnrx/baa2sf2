#pragma once

#include <istream>
#include <optional>

#include "AudioArchive.hpp"

#include "Poco/BinaryReader.h"
#include "Poco/Logger.h"

namespace z2sound
{

class AudioArchiveLoader
{
public:
  AudioArchiveLoader(std::istream& stream, Poco::Logger& logger);
  std::optional<AudioArchive> load();

private:
  bool read_command();
  void skip_marker(uint32_t marker, size_t num_words);

  Poco::BinaryReader reader_;
  Poco::Logger& logger_;
};

}