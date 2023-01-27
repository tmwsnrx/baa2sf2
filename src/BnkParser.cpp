#include "BnkParser.hpp"

#include "utils.hpp"

static constexpr uint32_t MARKER_ENVT = 0x454e5654;
static constexpr uint32_t MARKER_OSCT = 0x4f534354;
static constexpr uint32_t MARKER_RAND = 0x52414e44;
static constexpr uint32_t MARKER_SENS = 0x53454e53;
static constexpr uint32_t MARKER_INST = 0x494e5354;
static constexpr uint32_t MARKER_PMAP = 0x504d4150;
static constexpr uint32_t MARKER_PERC = 0x50455243;
static constexpr uint32_t MARKER_LIST = 0x4c495354;

static constexpr uint32_t MARKER_PERC_ENTRY = 0x50657263;
static constexpr uint32_t MARKER_INST_ENTRY = 0x496e7374;
static constexpr uint32_t MARKER_PMAP_ENTRY = 0x506d6170;

namespace z2sound
{

  BnkParser::BnkParser(uint32_t group, std::istream &stream, std::streamoff base_offset, Poco::Logger &logger)
      : group_{group}, base_offset_{base_offset}, size_{0}, reader_{stream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER}, logger_{logger}
  {
  }

  std::optional<InstrumentBank> BnkParser::parse()
  {
    uint32_t id{};
    uint32_t version{};

    reader_ >> size_;
    reader_ >> id;
    reader_ >> version;

    if (version != 1)
    {
      logger_.warning("Unsupported BNK version %u. Only version 1 is supported", version);
      return std::nullopt;
    }

    logger_.debug("Start parsing BNK (group %u, ID %u, %u bytes)", group_, id, size_);

    reader_.stream().seekg(4 * sizeof(uint32_t), std::ios::cur);
    enumerate_chunks();

    logger_.debug("Found the following chunks:");
    for (const auto &[marker, chunk_info] : chunk_table_)
    {
      logger_.debug("%s (position=%x, size=%u)", marker_to_string(marker), static_cast<uint32_t>(chunk_info.position), chunk_info.size);
    }

    std::optional list_chunk = get_chunk_info(MARKER_LIST);
    std::optional envt_chunk = get_chunk_info(MARKER_ENVT);
    std::optional osct_chunk = get_chunk_info(MARKER_OSCT);

    if (!list_chunk.has_value())
    {
      report_missing_chunk(MARKER_LIST);
      return std::nullopt;
    }

    if (!envt_chunk.has_value())
    {
      report_missing_chunk(MARKER_ENVT);
      return std::nullopt;
    }

    if (!osct_chunk.has_value())
    {
      report_missing_chunk(MARKER_OSCT);
      return std::nullopt;
    }

    uint32_t num_instruments{};
    reader_.stream().seekg(list_chunk.value().get().position);
    reader_ >> num_instruments;

    std::streampos instrument_list_position = reader_.stream().tellg();

    for (size_t instrument = 0; instrument < num_instruments; instrument++)
    {
      reader_.stream().seekg(instrument_list_position + static_cast<std::streamoff>(instrument * sizeof(uint32_t)));

      uint32_t offset{};
      reader_ >> offset;

      if (offset == 0)
      {
        continue;
      }

      reader_.stream().seekg(base_offset_ + offset);

      uint32_t marker{};
      reader_ >> marker;

      if (marker == MARKER_INST_ENTRY)
      {
        parse_inst();
      }
      else if (marker == MARKER_PERC_ENTRY)
      {
        parse_perc();
      }
      else
      {
        logger_.warning("Unknown instrument type %s (entry %i)", marker_to_string(marker), instrument);
      }
    }

    logger_.debug("Found %u instruments", num_instruments);

    InstrumentBank bank;
    return bank;
  }

  void BnkParser::enumerate_chunks()
  {
    while (reader_.stream().tellg() < base_offset_ + size_)
    {
      uint32_t marker{};
      reader_ >> marker;

      if (marker == 0)
      {
        break;
      }

      uint32_t size{};
      reader_ >> size;

      chunk_table_.emplace(marker, ChunkInfo{
                                       .position = reader_.stream().tellg(),
                                       .size = size});

      uint32_t aligned_chunk_size = (size + sizeof(uint32_t) - 1) & ~(sizeof(uint32_t) - 1);
      reader_.stream().seekg(aligned_chunk_size, std::ios::cur);
    }
  }

  std::optional<std::reference_wrapper<const BnkParser::ChunkInfo>> BnkParser::get_chunk_info(uint32_t marker) const
  {
    if (chunk_table_.count(marker) == 1)
    {
      return std::cref(chunk_table_.at(marker));
    }

    return std::nullopt;
  }

  void BnkParser::report_missing_chunk(uint32_t marker)
  {
    logger_.error("BNK is missing %s chunk, which is mandatory", marker_to_string(marker));
  }

  BasicInstrument BnkParser::parse_inst()
  {
    BasicInstrument instrument;

    uint32_t num_osc_entries{};
    reader_ >> num_osc_entries;

    for (size_t i = 0; i < num_osc_entries; i++)
    {
      uint32_t osc_index{};
      reader_ >> osc_index;
      instrument.oscillator_indices_.push_back(osc_index);
    }

    uint32_t unknown_count{};
    reader_ >> unknown_count;
    reader_.stream().seekg(unknown_count * sizeof(uint32_t), std::ios::cur);

    uint32_t num_key_regions{};
    reader_ >> num_key_regions;

    for (size_t i = 0; i < num_key_regions; i++)
    {
      BasicInstrument::KeyRegion key_region;

      reader_ >> key_region.upper_key_limit;
      reader_.stream().seekg(3, std::ios::cur);

      uint32_t num_velocity_regions{};
      reader_ >> num_velocity_regions;
      reader_.stream().seekg(6, std::ios::cur);

      reader_ >> key_region.sample_id;
      reader_ >> key_region.volume_multiplier;
      reader_ >> key_region.pitch_multiplier;

      // Skip any additional velocity region
      reader_.stream().seekg((num_velocity_regions - 1) * 12);

      instrument.key_regions_.emplace_back(key_region);
    }

    reader_ >> instrument.volume_multiplier_;
    reader_ >> instrument.pitch_multiplier_;

    return instrument;
  }

  PercussionSet BnkParser::parse_perc()
  {
    PercussionSet percussion_set;
    return percussion_set;
  }

}