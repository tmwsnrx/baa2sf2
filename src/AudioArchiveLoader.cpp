#include "AudioArchiveLoader.hpp"

#include <cstdint>
#include <utility>

#include "BnkParser.hpp"
#include "InstrumentBank.hpp"
#include "utils.hpp"
#include "WsysParser.hpp"

static constexpr uint32_t MARKER_BEGIN_ARCHIVE = 0x41415f3c;
static constexpr uint32_t MARKER_END_ARCHIVE = 0x3e5f4141;

static constexpr uint32_t MARKER_BEGIN_BL = 0x626c5f3c;
static constexpr uint32_t MARKER_END_BL = 0x3e5f626c;

static constexpr uint32_t MARKER_BFCA = 0x62666361;
static constexpr uint32_t MARKER_BMS = 0x626d7320;
static constexpr uint32_t MARKER_BMSA = 0x626d7361;
static constexpr uint32_t MARKER_BNK = 0x626e6b20;
static constexpr uint32_t MARKER_BSC = 0x62736320;
static constexpr uint32_t MARKER_BSFT = 0x62736674;
static constexpr uint32_t MARKER_BST = 0x62737420;
static constexpr uint32_t MARKER_BSTN = 0x6273746e;
static constexpr uint32_t MARKER_DSQB = 0x64737162;
static constexpr uint32_t MARKER_SECT = 0x73656374;
static constexpr uint32_t MARKER_VBNK = 0x76626e6b;
static constexpr uint32_t MARKER_WS = 0x77732020;

namespace z2sound
{

AudioArchiveLoader::AudioArchiveLoader(std::istream& stream, Poco::Logger& logger)
: reader_{stream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER}, logger_{logger}
{}

std::optional<AudioArchive> AudioArchiveLoader::load()
{
  reader_.stream().seekg(0);

  uint32_t marker{};
  reader_ >> marker;

  if (marker != MARKER_BEGIN_ARCHIVE)
  {
    logger_.error("File is not a valid audio archive");
    return std::nullopt;
  }

  logger_.information("Found valid archive begin marker, start parsing");
  while (read_command());

  logger_.information("Done parsing");
  return std::move(archive_);
}

bool AudioArchiveLoader::read_command()
{
  uint32_t marker{};
  reader_ >> marker;

  switch (marker)
  {
    case MARKER_END_ARCHIVE:
      return false;

    case MARKER_BNK:
    {
      uint32_t group{};
      uint32_t offset{};

      reader_ >> group;
      reader_ >> offset;

      logger_.information("bnk (group=%u, offset=%x)", group, offset);

      save_position();
      read_bnk(group, offset);
      restore_position();

      break;
    }

    case MARKER_BSC:
    case MARKER_BST:
    case MARKER_BSTN:
      skip_marker(marker, 2);
      break;

    case MARKER_WS:
    {
      uint32_t group{};
      uint32_t offset{};

      reader_ >> group;
      reader_ >> offset;
      reader_.stream().seekg(4, std::ios::cur);

      logger_.information("ws (group=%u, offset=%x)", group, offset);

      save_position();
      read_wsys(group, offset);
      restore_position();

      break;
    }

    default:
      return false;
  }

  return true;
}

void AudioArchiveLoader::skip_marker(uint32_t marker, size_t num_words)
{
  logger_.debug("Skipping marker %s", marker_to_string(marker));
  reader_.stream().seekg(num_words * sizeof(uint32_t), std::ios::cur);
}

void AudioArchiveLoader::read_bnk(uint32_t group, uint32_t offset)
{
  reader_.stream().seekg(offset + sizeof(uint32_t), std::ios::beg);

  BnkParser bnk_parser{group, reader_.stream(), offset, logger_};
  auto instrument_bank = bnk_parser.parse();

  if (!instrument_bank.has_value())
  {
    logger_.error("Failed to parse IBNK");
    return;
  }

  archive_.instrument_banks_.emplace(instrument_bank->get_id(), std::move(*instrument_bank));
}

void AudioArchiveLoader::read_wsys(uint32_t bank_id, uint32_t offset)
{
  reader_.stream().seekg(offset + sizeof(uint32_t), std::ios::beg);

  WsysParser wsys_parser{bank_id, reader_.stream(), offset, logger_};
  auto wave_bank = wsys_parser.parse();

  if (!wave_bank.has_value())
  {
    logger_.error("Failed to parse WSYS %u", bank_id);
    return;
  }

  archive_.wave_banks_.emplace(wave_bank->get_id(), std::move(*wave_bank));
}

void AudioArchiveLoader::save_position()
{
  saved_position_ = reader_.stream().tellg();
}

void AudioArchiveLoader::restore_position()
{
  reader_.stream().seekg(saved_position_, std::ios::beg);
}

}