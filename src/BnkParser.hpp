#pragma once

#include <cstdint>
#include <functional>
#include <istream>
#include <map>
#include <optional>

#include "Poco/BinaryReader.h"
#include "Poco/Logger.h"

#include "BasicInstrument.hpp"
#include "InstrumentBank.hpp"
#include "PercussionSet.hpp"

namespace z2sound
{

class BnkParser
{
public:
  BnkParser(uint32_t group, std::istream& stream, std::streamoff base_offset, Poco::Logger& logger);
  std::optional<InstrumentBank> parse();

private:
  struct ChunkInfo
  {
    std::streamoff position;
    uint32_t size;
  };

  void enumerate_chunks();
  std::optional<std::reference_wrapper<const ChunkInfo>> get_chunk_info(uint32_t marker) const;
  void report_missing_chunk(uint32_t marker);

  BasicInstrument parse_inst();
  PercussionSet parse_perc();

  uint32_t group_;
  std::streamoff base_offset_;
  uint32_t size_;
  std::map<uint32_t, ChunkInfo> chunk_table_;

  Poco::BinaryReader reader_;
  Poco::Logger& logger_;
};

}
