#pragma once

#include <istream>
#include <optional>

#include "AudioArchive.hpp"
#include "Poco/BinaryReader.h"

namespace z2sound
{

class AudioArchiveLoader
{
public:
  explicit AudioArchiveLoader(std::istream& stream);
  std::optional<AudioArchive> load();

private:
  bool read_command();

  Poco::BinaryReader reader_;
};

}