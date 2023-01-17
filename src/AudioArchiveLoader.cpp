#include <cstdint>

#include "AudioArchiveLoader.hpp"

static constexpr uint32_t MARKER_BEGIN_ARCHIVE = 0x41415f3c;
static constexpr uint32_t MARKER_END_ARCHIVE = 0x3e5f4141;

static constexpr uint32_t MARKER_BEGIN_BL = 0x626c5f3c;
static constexpr uint32_t MARKER_END_BL = 0x3e5f626c;

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

AudioArchiveLoader::AudioArchiveLoader(std::istream& stream)
: reader_{stream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER}
{}

std::optional<AudioArchive> AudioArchiveLoader::load()
{
  reader_.stream().seekg(0);

  AudioArchive archive;

  uint32_t marker{};
  reader_ >> marker;

  if (marker != MARKER_BEGIN_ARCHIVE)
  {
    return std::nullopt;
  }

  while (read_command());

  return archive;
}

bool AudioArchiveLoader::read_command()
{
  return false;
}

}