#include "BnkParser.hpp"

namespace z2sound
{

BnkParser::BnkParser(uint32_t group, std::istream& stream, Poco::Logger& logger)
: group_{group}, reader_{stream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER}, logger_{logger}
{
}

std::optional<InstrumentBank> BnkParser::parse()
{
  uint32_t size{};
  uint32_t id{};
  uint32_t version{};

  reader_ >> size;
  reader_ >> id;
  reader_ >> version;

  if (version != 1)
  {
    logger_.warning("Unsupported BNK version %u. Only version 1 is supported", version);
    return std::nullopt;
  }

  logger_.debug("Start parsing BNK (group %u, ID %u, %u bytes)", group_, id, size);

  InstrumentBank bank;
}

}